
#include "gui/widget.h"

using namespace myos::common;
using namespace myos::gui;

class Widget
        {
            protected:
                Widget *parent;
                common::int32_t x;
                common::int32_t y;
                common::int32_t w;
                common::int32_t h;

                common::uint8_t r;
                common::uint8_t g;
                common::uint8_t b;
        
            public:
Widget::Widget(Widget *parent, int32_t y, int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b)
{
    this->parent = parent;

    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    this->r = r;
    this->g = g;
    this->b = b;

    this->Focussable = true;
}

Widget::~Widget()
{

}

void Widget::GetFocus(Widget *widget)
{
    if(parent != 0)
    {
        parent->GetFocus(widget);
    }
}
void Widget::ModelToScreen(int32_t &x, int32_t &y)
{
    if(parent != 0)
    {
        parent->ModelToScreen(x, y);
    }

    x += this->x;
    y += this->y;
}

void Widget::Draw(GraphicsContext *gc)
{
    int X = x;
    int Y = y;
    ModelToScreen(X, Y);
    gc->FillRectangle(X, Y, w, h, r, g, b);
}

void Widget::OnMouseDown(int32_t x, int32_t y)
{
    if(Focussable)
    {
        GetFocus(this);
    }
}

void Widget::OnMouseUp(int32_t x, int32_t y)
{

}

void Widget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx, int32_t newy)
{

}

void Widget::OnKeyDown(char *str)
{

}

void Widget::OnKeyUp(char *str)
{

}

CompositeWidget::CompositeWidget(Widget *parent, int32_t y, int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b)
{
    focussedChild = 0;
    numChildren   = 0;
}

CompositeWidget::~CompositeWidget()
{

}

void CompositeWidget::GetFocus(Widget *widget)
{
    this->focussedChild = widget;

    if(parent != 0)
    {
        parent->GetFocus(this);
    }
}

void CompositeWidget::Draw(GraphicsContext *gc)
{
    Widget::Draw(gc);
    for(int i=numChildren-1; i>=0; i--)
    {
        children[i]->Draw(gc);
    }
}
void CompositeWidget::OnMouseDown(int32_t x, int32_t y)
{
    for(int i=0; i<numChildren; i++)
    {
        if(children[i]->ContainsCoordinate(x - this->x, y - this->y))
        {
            children[i]->OnMouseDown(x - this->x, y - this->y);
            break;
        }
    }
}
void CompositeWidget::OnMouseUp(int32_t x, int32_t y)
{
    for(int i=0; i<numChildren; i++)
    {
        if(children[i]->ContainsCoordinate(x - this->x, y - this->y))
        {
            children[i]->OnMouseUp(x - this->x, y - this->y);
            break;
        }
    }
}
void CompositeWidget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx, int32_t newy)
{
    int firstchild = -1;

    for(int i=0; i<numChildren; i++)
    {
        if(children[i]->ContainsCoordinate(oldx - this->x, oldy - this->y))
        {
            children[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            firstchild = i;
            break;
        }
    }

    for(int i=0; i<numChildren; i++)
    {
        if(children[i]->ContainsCoordinate(newx - this->x, newy - this->y))
        {
            if(firstchild != i)
            {
                children[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            }
            break;
        }
    }
}
void CompositeWidget::OnKeyDown(char *str)
{
    if(focussedChild != 0)
    {
        focussedChild->OnKeyDown(str);
    }
}
void CompositeWidget::OnKeyUp(char *str)
{
    if(focussedChild != 0)
    {
        focussedChild->OnKeyUp(str);
    }
}
