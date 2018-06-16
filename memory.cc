#include <memory>
struct Canvas;

class Circle
{
public:
        Circle(Canvas* canvas);  // "constructor"
        void position(int x, int y);
        void paint();
private:
        int d_x, d_y;
        int d_size;
        Canvas* d_canvas;
};

Circle::Circle(Canvas* canvas)
{
        d_canvas = canvas;
}

void Circle::paint()
{
//        d_canvas->drawCircle(d_x, d_y, d_size);
}

int main()
{
        Canvas* canvas=0;
        std::shared_ptr<Circle> ptr(new Circle(canvas));
        // or better:
        auto aptr = std::make_shared<Circle>(canvas);
}

