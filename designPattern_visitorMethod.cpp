// ============================================================
// Visitor Pattern Notes
// ============================================================
// Intent:
// Visitor is a behavioral design pattern that lets you separate algorithms
// from the objects on which they operate.
//
// Core idea:
// 1) Keep object structure stable (elements).
// 2) Put cross-cutting operations into visitor classes.
// 3) Use double dispatch: element.accept(visitor) -> visitor.visit(ElementType).
//
// When to use:
// 1) You need to run multiple operations on a fixed object hierarchy.
// 2) You want to add new behaviors without modifying element classes often.
// 3) You need to traverse complex structures (like composite trees).
//
// Advantages:
// 1) Open/Closed for adding new operations (new visitors).
// 2) Keeps element classes focused on core responsibilities.
// 3) Easy to collect cross-element statistics during traversal.
//
// Trade-offs:
// 1) Adding a new element type requires updating all visitors.
// 2) Visitor may need more exposure to element internals.
//
// ============================================================
// 访问者模式笔记
// ============================================================
// 意图：
// 访问者模式是一种行为型设计模式，用于将“算法”与“对象结构”解耦。
//
// 核心思想：
// 1）对象结构（元素层次）尽量稳定。
// 2）把横切行为抽到访问者类。
// 3）通过双分派：element.accept(visitor) 再分派到对应 visit 方法。
//
// 适用场景：
// 1）对象层次稳定，但需要不断新增操作。
// 2）希望新增行为时尽量不改元素类。
// 3）需要在复杂结构（如组合树）上执行统一操作。
//
// 优点：
// 1）新增操作只需增加新的 Visitor。
// 2）元素类更聚焦主职责。
// 3）便于在遍历时收集统计信息。
//
// 缺点：
// 1）新增元素类型时，所有 Visitor 都要补对应 visit 方法。
// 2）访问者可能需要更多元素内部信息。

#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/*
Scenario Explanation:

We model a small graphics editor with shape hierarchy:
Dot, Circle, Rectangle, and CompoundShape.

Then we add two independent behaviors via visitors:
1) XMLExportVisitor      -> export shape data as XML-like text.
2) AreaSummaryVisitor    -> compute total area over all visited shapes.

This shows how we can add operations without changing shape business logic.
*/

/*
场景说明：

本例模拟一个小型图形编辑器，元素层次包含：
Dot、Circle、Rectangle、CompoundShape。

在不改图形核心逻辑的前提下，新增两种行为：
1）XMLExportVisitor   -> 导出 XML 风格描述。
2）AreaSummaryVisitor -> 统计访问到的图形总面积。

这体现了“对象结构稳定、行为可扩展”的访问者模式价值。
*/

class Dot;
class Circle;
class Rectangle;
class CompoundShape;

class Visitor
{
public:
  virtual ~Visitor() = default;
  virtual void visitDot(const Dot &shape) = 0;
  virtual void visitCircle(const Circle &shape) = 0;
  virtual void visitRectangle(const Rectangle &shape) = 0;
  virtual void visitCompoundShape(const CompoundShape &shape) = 0;
};

class Shape
{
public:
  virtual ~Shape() = default;
  virtual void accept(Visitor &visitor) const = 0;
};

class Dot : public Shape
{
public:
  Dot(int idValue, double xValue, double yValue)
    : id(idValue), x(xValue), y(yValue) {}

  void accept(Visitor &visitor) const override
  {
    visitor.visitDot(*this);
  }

  int getId() const { return id; }
  double getX() const { return x; }
  double getY() const { return y; }

private:
  int id;
  double x;
  double y;
};

class Circle : public Shape
{
public:
  Circle(int idValue, double xValue, double yValue, double radiusValue)
    : id(idValue), x(xValue), y(yValue), radius(radiusValue) {}

  void accept(Visitor &visitor) const override
  {
    visitor.visitCircle(*this);
  }

  int getId() const { return id; }
  double getX() const { return x; }
  double getY() const { return y; }
  double getRadius() const { return radius; }

private:
  int id;
  double x;
  double y;
  double radius;
};

class Rectangle : public Shape
{
public:
  Rectangle(int idValue, double xValue, double yValue, double widthValue, double heightValue)
    : id(idValue), x(xValue), y(yValue), width(widthValue), height(heightValue) {}

  void accept(Visitor &visitor) const override
  {
    visitor.visitRectangle(*this);
  }

  int getId() const { return id; }
  double getX() const { return x; }
  double getY() const { return y; }
  double getWidth() const { return width; }
  double getHeight() const { return height; }

private:
  int id;
  double x;
  double y;
  double width;
  double height;
};

class CompoundShape : public Shape
{
public:
  explicit CompoundShape(int idValue) : id(idValue) {}

  void add(unique_ptr<Shape> child)
  {
    children.push_back(std::move(child));
  }

  void accept(Visitor &visitor) const override
  {
    visitor.visitCompoundShape(*this);
  }

  int getId() const { return id; }
  const vector<unique_ptr<Shape>> &getChildren() const { return children; }

private:
  int id;
  vector<unique_ptr<Shape>> children;
};

class XMLExportVisitor : public Visitor
{
public:
  void visitDot(const Dot &shape) override
  {
    xml << "<dot id='" << shape.getId()
      << "' x='" << shape.getX()
      << "' y='" << shape.getY() << "'/>\n";
  }

  void visitCircle(const Circle &shape) override
  {
    xml << "<circle id='" << shape.getId()
      << "' x='" << shape.getX()
      << "' y='" << shape.getY()
      << "' radius='" << shape.getRadius() << "'/>\n";
  }

  void visitRectangle(const Rectangle &shape) override
  {
    xml << "<rectangle id='" << shape.getId()
      << "' x='" << shape.getX()
      << "' y='" << shape.getY()
      << "' width='" << shape.getWidth()
      << "' height='" << shape.getHeight() << "'/>\n";
  }

  void visitCompoundShape(const CompoundShape &shape) override
  {
    xml << "<compound id='" << shape.getId() << "'>\n";
    for (const auto &child : shape.getChildren())
    {
      child->accept(*this);
    }
    xml << "</compound>\n";
  }

  string result() const
  {
    return xml.str();
  }

private:
  ostringstream xml;
};

class AreaSummaryVisitor : public Visitor
{
public:
  void visitDot(const Dot &) override
  {
  }

  void visitCircle(const Circle &shape) override
  {
    totalArea += 3.14159265358979323846 * shape.getRadius() * shape.getRadius();
  }

  void visitRectangle(const Rectangle &shape) override
  {
    totalArea += shape.getWidth() * shape.getHeight();
  }

  void visitCompoundShape(const CompoundShape &shape) override
  {
    for (const auto &child : shape.getChildren())
    {
      child->accept(*this);
    }
  }

  double getTotalArea() const
  {
    return totalArea;
  }

private:
  double totalArea = 0.0;
};

int main()
{
  vector<unique_ptr<Shape>> scene;

  scene.push_back(make_unique<Dot>(1, 5, 10));
  scene.push_back(make_unique<Circle>(2, 10, 10, 3));
  scene.push_back(make_unique<Rectangle>(3, 0, 0, 8, 6));

  auto group = make_unique<CompoundShape>(10);
  group->add(make_unique<Circle>(11, 20, 15, 2));
  group->add(make_unique<Rectangle>(12, 2, 3, 4, 5));
  scene.push_back(std::move(group));

  XMLExportVisitor xmlVisitor;
  AreaSummaryVisitor areaVisitor;

  for (const auto &shape : scene)
  {
    shape->accept(xmlVisitor);
    shape->accept(areaVisitor);
  }

  cout << "=== XML Export ===\n";
  cout << xmlVisitor.result() << "\n";

  cout << "=== Area Summary ===\n";
  cout << "total area = " << areaVisitor.getTotalArea() << "\n";

  return 0;
}