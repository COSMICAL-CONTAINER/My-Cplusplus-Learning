#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/*
Iterator Pattern Notes (Overview)

Iterator is a behavioral design pattern that lets you traverse elements of a collection
without exposing its internal representation (list, tree, graph, etc.). It extracts
traversal logic into dedicated iterator objects.

Core idea:
1) Collection exposes factory methods to create iterators.
2) Iterator encapsulates traversal state and algorithm.
3) Client consumes collection elements through iterator interface only.

When to use Iterator:
1) Collection internals are complex and should remain hidden.
2) You need multiple traversal strategies over the same data.
3) You want to avoid duplicating traversal code in business logic.

Advantages:
1) Separates traversal concerns from collection and client code (SRP).
2) Supports OCP by adding new iterators without changing clients.
3) Multiple iterators can traverse the same collection independently.
4) Iteration can be paused/resumed using iterator state.

Trade-offs:
1) May be overkill for very simple collections.
2) Some custom iterator implementations can introduce overhead.

Relationship with other patterns:
1) Frequently used with Composite to traverse object trees.
2) Factory Method can be used to return iterator variants per collection type.
3) Visitor + Iterator can apply operations to heterogeneous structures.
*/

/*
迭代器模式笔记（概览）

迭代器（Iterator）是一种行为型设计模式，
它允许你在不暴露集合底层结构（列表、树、图等）的前提下遍历集合元素。
遍历相关逻辑会被抽取到独立的迭代器对象中。

核心思想：
1）集合通过工厂方法创建迭代器。
2）迭代器封装遍历算法与遍历状态。
3）客户端只依赖迭代器接口访问元素。

适用场景：
1）集合内部结构较复杂，需要对客户端隐藏细节。
2）同一数据需要多种遍历方式。
3）希望避免在业务代码中重复写遍历逻辑。

优点：
1）遍历职责与集合/客户端职责分离，符合 SRP。
2）新增迭代器无需改客户端，符合 OCP。
3）多个迭代器可并行、独立遍历同一集合。
4）可基于状态实现暂停/继续遍历。

缺点：
1）对非常简单集合可能显得过度设计。
2）某些自定义迭代器会带来额外开销。

与其他模式关系：
1）常与组合模式结合遍历对象树。
2）可与工厂方法结合返回不同迭代器变体。
3）可与访问者结合处理异构结构中的元素。
*/

/*
Scenario Explanation:

This file uses a social network traversal scenario.
The collection (SocialNetwork) stores profile relationships but does not expose its
internal data structure directly. It provides two iterator factories:
createFriendsIterator(profileId) and createCoworkersIterator(profileId).
Different iterator implementations traverse the same data using different filters,
while client code (SocialSpammer) stays unchanged.
*/

/*
场景说明：

本文件使用“社交网络关系遍历”场景。
集合（SocialNetwork）保存档案关系数据，但不直接暴露内部结构。
它提供两个迭代器工厂方法：createFriendsIterator(profileId)
和 createCoworkersIterator(profileId)。
不同迭代器在同一份数据上按不同规则遍历（好友/同事过滤），
客户端（SocialSpammer）代码无需改动。
*/

struct Profile
{
    string id;
    string name;
    string company;
    string email;
};

// Iterator interface.
// 迭代器接口。
class ProfileIterator
{
public:
    virtual ~ProfileIterator() = default;
    virtual bool hasMore() = 0;
    virtual Profile getNext() = 0;
};

// Collection interface.
// 集合接口。
class SocialNetwork
{
public:
    virtual ~SocialNetwork() = default;
    virtual unique_ptr<ProfileIterator> createFriendsIterator(const string &profileId) = 0;
    virtual unique_ptr<ProfileIterator> createCoworkersIterator(const string &profileId) = 0;
};

class WeChat;

// Concrete iterator for WeChat relations.
// WeChat 关系的具体迭代器。
class WeChatIterator : public ProfileIterator
{
public:
    WeChatIterator(const WeChat &networkRef, string ownerId, string relationType);

    bool hasMore() override;
    Profile getNext() override;

private:
    void lazyInit();

private:
    const WeChat &network;
    string profileId;
    string type; // "friends" or "coworkers"
    size_t currentPosition = 0;
    vector<Profile> cache;
    bool initialized = false;
};

class WeChat : public SocialNetwork
{
public:
    WeChat()
    {
        profiles = {
            {"u100", "Alice", "Acme", "alice@acme.com"},
            {"u101", "Bob", "Acme", "bob@acme.com"},
            {"u102", "Cindy", "Globex", "cindy@globex.com"},
            {"u103", "David", "Acme", "david@acme.com"},
            {"u104", "Eva", "Innotech", "eva@innotech.com"}};

        friendships = {
            {"u100", {"u101", "u102", "u103"}},
            {"u101", {"u100", "u104"}},
            {"u102", {"u100"}},
            {"u103", {"u100"}},
            {"u104", {"u101"}}};
    }

    unique_ptr<ProfileIterator> createFriendsIterator(const string &profileId) override
    {
        return make_unique<WeChatIterator>(*this, profileId, "friends");
    }

    unique_ptr<ProfileIterator> createCoworkersIterator(const string &profileId) override
    {
        return make_unique<WeChatIterator>(*this, profileId, "coworkers");
    }

    vector<Profile> socialGraphRequest(const string &profileId, const string &type) const
    {
        vector<Profile> result;

        auto it = friendships.find(profileId);
        if (it == friendships.end())
        {
            return result;
        }

        const Profile *owner = findProfile(profileId);
        if (!owner)
        {
            return result;
        }

        for (const string &friendId : it->second)
        {
            const Profile *candidate = findProfile(friendId);
            if (!candidate)
            {
                continue;
            }

            if (type == "friends")
            {
                result.push_back(*candidate);
            }
            else if (type == "coworkers" && candidate->company == owner->company)
            {
                result.push_back(*candidate);
            }
        }

        return result;
    }

private:
    const Profile *findProfile(const string &profileId) const
    {
        for (const auto &profile : profiles)
        {
            if (profile.id == profileId)
            {
                return &profile;
            }
        }
        return nullptr;
    }

private:
    vector<Profile> profiles;
    unordered_map<string, vector<string>> friendships;

    friend class WeChatIterator;
};

WeChatIterator::WeChatIterator(const WeChat &networkRef, string ownerId, string relationType)
    : network(networkRef), profileId(std::move(ownerId)), type(std::move(relationType)) {}

void WeChatIterator::lazyInit()
{
    if (!initialized)
    {
        cache = network.socialGraphRequest(profileId, type);
        initialized = true;
    }
}

bool WeChatIterator::hasMore()
{
    lazyInit();
    return currentPosition < cache.size();
}

Profile WeChatIterator::getNext()
{
    lazyInit();
    if (!hasMore())
    {
        return {};
    }
    return cache[currentPosition++];
}

class SocialSpammer
{
public:
    void send(ProfileIterator &iterator, const string &message)
    {
        while (iterator.hasMore())
        {
            Profile profile = iterator.getNext();
            cout << "Send to " << profile.email << " -> " << message << "\n";
        }
    }
};

int main()
{
    WeChat network;
    SocialSpammer spammer;

    cout << "Friends iterator:\n";
    auto friendsIt = network.createFriendsIterator("u100");
    spammer.send(*friendsIt, "Important message for friends");

    cout << "\nCoworkers iterator:\n";
    auto coworkersIt = network.createCoworkersIterator("u100");
    spammer.send(*coworkersIt, "Important message for coworkers");

    return 0;
}
