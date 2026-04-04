// ============================================================
// Template Method Pattern Notes
// ============================================================
// Intent:
// Template Method is a behavioral pattern that defines the skeleton of an
// algorithm in a base class and lets subclasses redefine specific steps
// without changing the algorithm structure.
//
// Core idea:
// 1) Put fixed workflow order in one template method.
// 2) Put variable steps into abstract/overridable methods.
// 3) Optional hooks provide extension points before/after key steps.
//
// When to use:
// 1) Multiple classes share same workflow but differ in some steps.
// 2) You want to reuse common logic and avoid duplicate code.
// 3) You must keep algorithm order stable and controlled by base class.
//
// Advantages:
// 1) Reuses common flow and reduces duplication.
// 2) Keeps high-level process consistent.
// 3) Subclasses only focus on customized steps.
//
// Trade-offs:
// 1) Inheritance coupling may increase.
// 2) Too many steps can make base class harder to maintain.
//
// Relation with Strategy:
// - Template Method changes behavior via inheritance (compile-time structure).
// - Strategy changes behavior via composition (runtime switching).
//
// ============================================================
// 模板方法模式笔记
// ============================================================
// 意图：
// 模板方法是一种行为型模式，在基类中定义算法骨架，
// 并允许子类在不改变整体结构的前提下重写特定步骤。
//
// 核心思想：
// 1）在模板方法中固定流程顺序。
// 2）把可变步骤放到抽象/可重写方法中。
// 3）通过钩子（hook）在关键步骤前后扩展。
//
// 适用场景：
// 1）多个类流程一致但细节步骤不同。
// 2）希望复用公共逻辑、减少重复代码。
// 3）希望流程顺序由基类统一控制。
//
// 优点：
// 1）复用流程，降低重复。
// 2）保持高层流程一致。
// 3）子类仅需关注差异步骤。
//
// 缺点：
// 1）继承耦合会增加。
// 2）步骤太多时基类维护成本上升。
//
// 与策略模式关系：
// - 模板方法通过继承改变行为（结构静态）。
// - 策略通过组合切换行为（运行时动态）。

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

/*
Scenario Explanation:

We model a document-mining system for DOC / CSV / PDF files.
All miners follow the same workflow:
open file -> extract raw data -> parse data -> analyze -> generate report -> close file.

Format-specific steps are implemented by subclasses,
while shared analysis/report logic stays in the base class.
*/

/*
场景说明：

本例模拟文档数据挖掘系统，支持 DOC / CSV / PDF 三种格式。
所有挖掘器遵循同一流程：
打开文件 -> 提取原始数据 -> 解析数据 -> 分析 -> 生成报告 -> 关闭文件。

格式差异由子类实现，公共分析与报告逻辑由基类复用。
*/

class DataMiner
{
public:
  virtual ~DataMiner() = default;

  void mine(const string &filePath)
  {
    openFile(filePath);
    beforeExtractHook(filePath);

    string rawData = extractRawData(filePath);
    vector<string> records = parseData(rawData);

    analyzeData(records);
    generateReport(records, filePath);

    afterReportHook(filePath);
    closeFile(filePath);
  }

protected:
  virtual string minerName() const = 0;
  virtual string extractRawData(const string &filePath) = 0;
  virtual vector<string> parseData(const string &rawData) = 0;

  virtual void beforeExtractHook(const string &) {}
  virtual void afterReportHook(const string &) {}

  void openFile(const string &filePath)
  {
    cout << "[" << minerName() << "] open file: " << filePath << "\n";
  }

  void closeFile(const string &filePath)
  {
    cout << "[" << minerName() << "] close file: " << filePath << "\n";
  }

  void analyzeData(const vector<string> &records)
  {
    cout << "[" << minerName() << "] analyze " << records.size() << " records\n";
  }

  void generateReport(const vector<string> &records, const string &filePath)
  {
    cout << "[" << minerName() << "] report for " << filePath << ": ";
    if (records.empty())
    {
      cout << "no valid records\n";
      return;
    }

    cout << "first record='" << records.front()
       << "', last record='" << records.back() << "'\n";
  }
};

class DocDataMiner : public DataMiner
{
protected:
  string minerName() const override
  {
    return "DOC Miner";
  }

  string extractRawData(const string &)
  {
    return "Title:Q1 Summary\nAuthor:TeamA\nParagraph:Revenue up";
  }

  vector<string> parseData(const string &rawData) override
  {
    vector<string> records;
    string token;
    for (char ch : rawData)
    {
      if (ch == '\n')
      {
        if (!token.empty())
        {
          records.push_back(token);
          token.clear();
        }
      }
      else
      {
        token.push_back(ch);
      }
    }
    if (!token.empty())
    {
      records.push_back(token);
    }
    return records;
  }
};

class CsvDataMiner : public DataMiner
{
protected:
  string minerName() const override
  {
    return "CSV Miner";
  }

  string extractRawData(const string &)
  {
    return "date,city,value;2026-03-01,beijing,120;2026-03-02,shanghai,150";
  }

  vector<string> parseData(const string &rawData) override
  {
    vector<string> records;
    string token;
    for (char ch : rawData)
    {
      if (ch == ';')
      {
        records.push_back(token);
        token.clear();
      }
      else
      {
        token.push_back(ch);
      }
    }
    if (!token.empty())
    {
      records.push_back(token);
    }
    return records;
  }

  void beforeExtractHook(const string &filePath) override
  {
    cout << "[CSV Miner] validate delimiter settings for: " << filePath << "\n";
  }
};

class PdfDataMiner : public DataMiner
{
protected:
  string minerName() const override
  {
    return "PDF Miner";
  }

  string extractRawData(const string &)
  {
    return "Page1|Invoice#9081|Amount:3999|Status:PAID";
  }

  vector<string> parseData(const string &rawData) override
  {
    vector<string> records;
    string token;
    for (char ch : rawData)
    {
      if (ch == '|')
      {
        records.push_back(token);
        token.clear();
      }
      else
      {
        token.push_back(ch);
      }
    }
    if (!token.empty())
    {
      records.push_back(token);
    }
    return records;
  }

  void afterReportHook(const string &filePath) override
  {
    cout << "[PDF Miner] archive metadata for: " << filePath << "\n";
  }
};

int main()
{
  unique_ptr<DataMiner> docMiner = make_unique<DocDataMiner>();
  unique_ptr<DataMiner> csvMiner = make_unique<CsvDataMiner>();
  unique_ptr<DataMiner> pdfMiner = make_unique<PdfDataMiner>();

  cout << "\n=== Run DOC Miner ===\n";
  docMiner->mine("report.doc");

  cout << "\n=== Run CSV Miner ===\n";
  csvMiner->mine("traffic.csv");

  cout << "\n=== Run PDF Miner ===\n";
  pdfMiner->mine("invoice.pdf");

  return 0;
}