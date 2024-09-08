// composite.hpp
#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include "shape.hpp"
#include <vector>
#include <memory>

class Composite : public Shape {
public:
    Composite(const std::string& name);
    ~Composite() override = default;

    void add(std::shared_ptr<Shape> shape) override;
    void remove(std::shared_ptr<Shape> shape) override;

    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os) const override;

    // Hinzufügen von begin() und end() Methoden
    auto begin() { return children_.begin(); }
    auto end() { return children_.end(); }
    auto begin() const { return children_.begin(); }
    auto end() const { return children_.end(); }

    // Getter für children_
    const std::vector<std::shared_ptr<Shape>>& getChildren() const { return children_; }

    Hitpoint intersectImpl(Ray const& ray) const override;

protected:
    

private:
    std::vector<std::shared_ptr<Shape>> children_;
};

#endif // COMPOSITE_HPP