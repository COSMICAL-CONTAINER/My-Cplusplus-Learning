#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

/*
Proxy Pattern Notes (Overview)

Proxy is a structural design pattern that provides a substitute (placeholder) for another
object. The proxy implements the same interface as the real service, controls access to it,
and can perform extra logic before/after forwarding requests.

Typical responsibilities of a proxy:
1) Lazy initialization (virtual proxy)
2) Access control (protection proxy)
3) Network indirection (remote proxy)
4) Caching (cache proxy)
5) Logging/metrics (logging proxy)

When to use Proxy:
1) Service object is heavy or expensive and should be created lazily.
2) You need centralized access checks, logging, or request control.
3) You want request result caching without changing service class code.

Advantages:
1) Controls service usage transparently to clients.
2) Adds cross-cutting behavior without modifying service implementation.
3) Can manage service lifecycle if needed.

Trade-offs:
1) Adds indirection and extra classes.
2) May increase response latency in some cases.
3) Incorrect cache/proxy policy can produce stale or inconsistent behavior.

Relationship with other patterns:
1) Adapter changes interface; Proxy keeps the same interface.
2) Decorator also wraps objects but focuses on behavior extension, not access control/lifecycle.
3) Facade simplifies subsystem APIs with a different interface; Proxy usually remains interchangeable with service.
*/

/*
浠ｇ悊妯″紡绗旇锛堟瑙堬級

浠ｇ悊锛圥roxy锛夋槸涓€绉嶇粨鏋勫瀷璁捐妯″紡锛岀敤浜庝负鏌愪釜瀵硅薄鎻愪緵鈥滄浛浠ｇ墿/鍗犱綅绗︹€濄€�
浠ｇ悊涓庣湡瀹炴湇鍔″疄鐜扮浉鍚屾帴鍙ｏ紝瀵瑰淇濇寔鍙浛鎹㈡€э紱瀹冨彲鍦ㄨ浆鍙戣姹傚墠鍚庡姞鍏ラ澶栭€昏緫锛�
渚嬪寤惰繜鍒濆鍖栥€佽闂帶鍒躲€佺紦瀛樸€佹棩蹇楃瓑銆�

浠ｇ悊鐨勫父瑙佽亴璐ｏ細
1锛夊欢杩熷垵濮嬪寲锛堣櫄鎷熶唬鐞嗭級
2锛夎闂帶鍒讹紙淇濇姢浠ｇ悊锛�
3锛夎繙绋嬭皟鐢ㄥ皝瑁咃紙杩滅▼浠ｇ悊锛�
4锛夌粨鏋滅紦瀛橈紙缂撳瓨浠ｇ悊锛�
5锛夋棩蹇椾笌鐩戞帶锛堟棩蹇椾唬鐞嗭級

閫傜敤鍦烘櫙锛�
1锛夋湇鍔″璞″緢閲嶏紝閫傚悎鎸夐渶鍒涘缓銆�
2锛夐渶瑕佺粺涓€鍋氶壌鏉冦€侀檺娴併€佹棩蹇楃瓑鎺у埗銆�
3锛夊笇鏈涘湪涓嶆敼鏈嶅姟绫荤殑鎯呭喌涓嬪姞鍏ョ紦瀛樸€�

浼樼偣锛�
1锛夊彲鍦ㄥ鎴风鏃犳劅鐭ヤ笅鎺у埗鏈嶅姟璁块棶銆�
2锛夎兘鍦ㄤ笉鏀规湇鍔″疄鐜扮殑鍓嶆彁涓嬪彔鍔犳í鍒囪兘鍔涖€�
3锛夊彲鎸夐渶绠＄悊鏈嶅姟瀵硅薄鐢熷懡鍛ㄦ湡銆�

缂虹偣锛�
1锛変細澧炲姞闂存帴灞傚拰绫绘暟閲忋€�
2锛夋煇浜涘満鏅彲鑳藉甫鏉ラ澶栧欢杩熴€�
3锛夌紦瀛樼瓥鐣ヨ璁′笉褰撲細瀵艰嚧鏁版嵁闄堟棫鎴栬涓轰笉涓€鑷淬€�

涓庡叾浠栨ā寮忓叧绯伙細
1锛夐€傞厤鍣ㄤ細鏀规帴鍙ｏ紱浠ｇ悊淇濇寔鍚屼竴鎺ュ彛銆�
2锛夎楗颁篃浼氬寘瑁呭璞★紝浣嗘牳蹇冩槸澧炲己琛屼负鑰岄潪璁块棶鎺у埗/鐢熷懡鍛ㄦ湡绠＄悊銆�
3锛夊瑙傞€氬父鎻愪緵涓嶅悓鐨勭畝鍖栨帴鍙ｏ紱浠ｇ悊閫氬父涓庢湇鍔″彲浜掓崲銆�
*/

/*
Scenario Explanation:

This file uses a third-party video service scenario.
ThirdPartyTVService simulates slow network operations. CachedTVProxy implements the same
interface and wraps the real service. It caches list/get requests and only forwards
network calls when needed, improving response speed and reducing repeated API traffic.
Client-side manager code depends only on the shared interface and can switch between
real service and proxy transparently.
*/

/*
鍦烘櫙璇存槑锛�

鏈枃浠朵娇鐢ㄢ€滅涓夋柟瑙嗛鏈嶅姟鈥濆満鏅瑙ｄ唬鐞嗘ā寮忋€�
ThirdPartyTVService 妯℃嫙鎱㈤€熺綉缁滆姹傦紱CachedTVProxy 涓庡叾瀹炵幇鐩稿悓鎺ュ彛骞惰繘琛屽寘瑁呫€�
浠ｇ悊浼氱紦瀛樿棰戝垪琛ㄤ笌瑙嗛璇︽儏锛岃姹傚懡涓椂鐩存帴杩斿洖缂撳瓨锛屾湭鍛戒腑鏃跺啀杞彂鍒扮湡瀹炴湇鍔★紝
浠庤€屽噺灏戦噸澶嶇綉缁滆皟鐢ㄥ苟鎻愬崌鍝嶅簲閫熷害銆傚鎴风绠＄悊鍣ㄥ彧渚濊禆缁熶竴鎺ュ彛锛屽彲鍦ㄧ湡瀹炴湇鍔′笌
浠ｇ悊涔嬮棿鏃犵紳鍒囨崲銆�
*/

// Service interface shared by real service and proxy.
// 鏈嶅姟鎺ュ彛锛氱湡瀹炴湇鍔′笌浠ｇ悊鍏卞悓瀹炵幇銆�
class IThirdPartyTVLib
{
public:
    virtual ~IThirdPartyTVLib() = default;

    virtual vector<string> listVideos() = 0;
    virtual string getVideoInfo(const string &id) = 0;
    virtual void downloadVideo(const string &id) = 0;
};

// Real service (simulates remote API).
// 鐪熷疄鏈嶅姟锛堟ā鎷熻繙绋� API锛夈€�
class ThirdPartyTVService : public IThirdPartyTVLib
{
public:
    vector<string> listVideos() override
    {
        cout << "[RealService] Fetching video list from remote server...\n";
        return {"cat-001", "cat-002", "cat-003"};
    }

    string getVideoInfo(const string &id) override
    {
        cout << "[RealService] Fetching video info for " << id << "...\n";
        return "VideoInfo{id=" + id + ", title=Funny Cats " + id + "}";
    }

    void downloadVideo(const string &id) override
    {
        cout << "[RealService] Downloading video file " << id << "...\n";
    }
};

// Proxy with caching and lazy service creation.
// 甯︾紦瀛樹笌鎳掑姞杞界殑浠ｇ悊銆�
class CachedTVProxy : public IThirdPartyTVLib
{
public:
    CachedTVProxy() = default;

    explicit CachedTVProxy(unique_ptr<IThirdPartyTVLib> serviceImpl)
        : service(std::move(serviceImpl)) {}

    vector<string> listVideos() override
    {
        ensureService();

        if (!listCacheValid)
        {
            listCache = service->listVideos();
            listCacheValid = true;
        }
        else
        {
            cout << "[Proxy] listVideos cache hit.\n";
        }

        return listCache;
    }

    string getVideoInfo(const string &id) override
    {
        ensureService();

        auto it = videoInfoCache.find(id);
        if (it != videoInfoCache.end())
        {
            cout << "[Proxy] getVideoInfo cache hit for " << id << ".\n";
            return it->second;
        }

        string info = service->getVideoInfo(id);
        videoInfoCache[id] = info;
        return info;
    }

    void downloadVideo(const string &id) override
    {
        ensureService();

        if (downloadedFiles.count(id) > 0)
        {
            cout << "[Proxy] download skipped (already cached): " << id << "\n";
            return;
        }

        service->downloadVideo(id);
        downloadedFiles.insert(id);
    }

    void resetCache()
    {
        listCacheValid = false;
        listCache.clear();
        videoInfoCache.clear();
        downloadedFiles.clear();
        cout << "[Proxy] cache reset.\n";
    }

private:
    void ensureService()
    {
        if (!service)
        {
            cout << "[Proxy] lazy init real service.\n";
            service = make_unique<ThirdPartyTVService>();
        }
    }

private:
    unique_ptr<IThirdPartyTVLib> service;

    bool listCacheValid = false;
    vector<string> listCache;
    unordered_map<string, string> videoInfoCache;
    unordered_set<string> downloadedFiles;
};

// Client class depends only on service interface.
// 瀹㈡埛绔鐞嗗櫒浠呬緷璧栨湇鍔℃帴鍙ｃ€�
class TVManager
{
public:
    explicit TVManager(IThirdPartyTVLib &tvService) : service(tvService) {}

    void renderListPanel()
    {
        vector<string> videos = service.listVideos();
        cout << "List panel:";
        for (const auto &id : videos)
        {
            cout << " " << id;
        }
        cout << "\n";
    }

    void renderVideoPage(const string &id)
    {
        cout << "Video page: " << service.getVideoInfo(id) << "\n";
    }

    void requestDownload(const string &id)
    {
        service.downloadVideo(id);
    }

private:
    IThirdPartyTVLib &service;
};

int main()
{
    CachedTVProxy proxy;
    TVManager manager(proxy);

    cout << "--- first round ---\n";
    manager.renderListPanel();
    manager.renderVideoPage("cat-001");
    manager.requestDownload("cat-001");

    cout << "\n--- second round (cache expected) ---\n";
    manager.renderListPanel();
    manager.renderVideoPage("cat-001");
    manager.requestDownload("cat-001");

    return 0;
}
