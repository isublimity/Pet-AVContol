#ifndef VFD16BT_H
#define VFD16BT_H

#ifndef PRIME_VFD_H
#define PRIME_VFD_H
class VFD16BT 
{

  public:
    VFD16BT(int pin_reset);
    ~VFD16BT();
    dump()
  private:
      byte _color;  // переменная цвета
      byte _bright; // переменная яркости
}
#endif // PRIME_VFD_H