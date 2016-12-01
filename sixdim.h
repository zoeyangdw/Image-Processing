#ifndef SIXDIM_H
#define SIXDIM_H

class SixDim
{
public:
    SixDim(int, int, double, double, double, double, double, double);
    bool operator<(const SixDim& sd) const;
    int getX();
    int getY();

private:
    double data[6];
    int x,y;
};

#endif // SIXDIM_H
