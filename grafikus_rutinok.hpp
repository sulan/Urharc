#ifndef GRAFIKUS_RUTINOK_HPP
#define GRAFIKUS_RUTINOK_HPP

#include <cmath>
#include <algorithm>
#include <vector>

//A nem egesz szamokkal valo muveleteknel a pontatlansag miatt
//lesz egy kis elteres az egyebkent egyenlo eredmenyek kozott
constexpr double EPSILON = 0.001;
constexpr double PI      = 3.141592654;
constexpr double INFTY   = 99999999999;

//===============================================
struct Vektor {
//===============================================
  double x,y;
  Vektor() : x{}, y{} {  }
  Vektor(double x_, double y_) : x{x_}, y{y_} {  }
  Vektor& operator+= (const Vektor& rhs) {
    x+=rhs.x; y+=rhs.y;
    return *this;
  }
  Vektor& operator-= (const Vektor& rhs) {
    x-=rhs.x; y-=rhs.y;
    return *this;
  }
  Vektor& operator*= (double rhs) {
    x*=rhs; y*=rhs;
    return *this;
  }
  bool operator== (const Vektor& rhs) const {
    return (std::abs(x-rhs.x)<EPSILON && std::abs(y-rhs.y)<EPSILON);
  }
  bool operator!= (const Vektor& rhs) const {
    return (!operator==(rhs));
  }
  double abs () const {
    return std::pow(std::pow(x,2)+std::pow(y,2),0.5);
  }
};

inline const Vektor operator+ (Vektor lhs, const Vektor& rhs) {
  return lhs+=rhs;
}

inline const Vektor operator- (Vektor lhs, const Vektor& rhs) {
  return lhs-=rhs;
}

inline const Vektor operator* (Vektor lhs, double rhs) {
  return lhs*=rhs;
}

inline const Vektor operator* (double lhs, Vektor rhs) {
  return rhs*=lhs;
}

//Skalaris szorzat
inline double operator* (Vektor lhs, Vektor rhs) {
  return lhs.x*rhs.x+lhs.y*rhs.y;
}

//===============================================
struct Box {
//===============================================
  explicit Box (Vektor bf_ = {}, Vektor ja_ = {})
    { setPos(bf_,ja_); }
  bool operator== (const Box& rhs) const
    {return (_bf==rhs._bf && _ja==rhs._ja);}
  bool operator!= (const Box& rhs) const
    {return !operator==(rhs);}
  Box& operator+= (Vektor rhs) {
    _bf += rhs; _ja += rhs;
    return *this;
  }
  Box& operator-= (Vektor rhs) {
    _bf -= rhs; _ja -= rhs;
    return *this;
  }
  double bal () const {return _bf.x;}
  double jobb () const {return _ja.x;}
  double felso () const {return _bf.y;}
  double also () const {return _ja.y;}
  double szeles () const {return jobb()-bal();}
  double magas () const {return also()-felso();}
  bool in (Vektor v) const {
    return (v.x>=bal() && v.x<jobb()
            && v.y>=felso() && v.y<also());
  }
  void setPos (Vektor ujbf, Vektor ujja) {
    if (ujbf.x>ujja.x) {std::swap(ujbf.x,ujja.x);}
    if (ujbf.y>ujja.y) {std::swap(ujbf.y,ujja.y);}
    _bf = ujbf; _ja=ujja;
  }
  Vektor bf() const {return _bf;}
  Vektor ja() const {return _ja;}
  Vektor jf() const {return {_ja.x,_bf.y};}
  Vektor ba() const {return {_bf.x,_ja.y};}
private:
  Vektor _bf,_ja;
};

inline const Box operator+ (Box lhs, Vektor rhs)
  { return lhs+=rhs; }

inline const Box operator- (Box lhs, Vektor rhs)
  { return lhs-=rhs; }


//===============================================
struct Egyenes {
//===============================================
  Vektor p0;
  //Radianban, a 0 felfele mutat, oramutato jarasanak megfeleloen
  explicit Egyenes (Vektor p0_ = {}, double irany_={})
    : p0 {p0_}, _irany{irany_} {  }
  explicit Egyenes (Vektor p0_ , Vektor irany_)
    : p0{p0_}, _irany{atan2(irany_.x,-1*irany_.y)} {  }
  Vektor iranyV () const {
    return Vektor{sin(_irany),-1*cos(_irany)};
  }
  Vektor normalV () const {
    return Vektor{cos(_irany),sin(_irany)};
  }
  double irany () const {return _irany;}
  void setIrany (double uj) {
    while (uj<0) {uj+=2*PI;}
    while (uj>=2*PI) {uj-=2*PI;}
    _irany = uj;
  }
  void setIrany (Vektor uj)
    {setIrany(atan2(uj.x,-1*uj.y));}
private:
  double _irany;
};

bool intersect (const Box&, const Box&);
//Clockwise
//bool intersect (const std::vector<Vektor>&, const std::Vector&);
bool parhuzamos (Vektor a, Vektor b);
bool parhuzamos (Egyenes a, Egyenes b);
//b a-tol merre van (origotol)
inline double _fordul (Vektor a, Vektor b) {
  return (a.x*b.y-a.y*b.x);
}
inline double fordul (Vektor a, Vektor b) {
  double i = _fordul(a,b);
  return i>0?1:(i==0?0:-1);
}
//c a-b-tol merre van
inline double _fordul (Vektor a, Vektor b, Vektor c) {
  return _fordul (b-a,c-a);
}
inline double fordul (Vektor a, Vektor b, Vektor c) {
  return fordul(b-a,c-a);
}
inline bool jobbra (Vektor a, Vektor b, Vektor c) {
  return _fordul(a,b,c)<0;
}
inline bool balra (Vektor a, Vektor b, Vektor c) {
  return _fordul(a,b,c)>0;
}
//c rajta van-e a-b-n
inline bool rajta (Vektor a, Vektor b, Vektor c) {
  return ( (std::abs(_fordul(a,b,c) <= EPSILON ))
        && ((c.x-a.x)*(c.x-b.x)<=EPSILON)
        && ((c.y-a.y)*(c.y-b.y)<=EPSILON) );
}
//a-b es c-d metszik-e egymast
inline bool metszi (Vektor a, Vektor b, Vektor c, Vektor d) {
  return ( ( (fordul(a,b,c)*fordul(a,b,d)<EPSILON)
          && (fordul(c,d,a)*fordul(c,d,b)<EPSILON) )
        || rajta(a,b,c) || rajta(a,b,d)
        || rajta(c,d,a) || rajta(c,d,b) );
}
//poligon tartalmazza-e a pontot
inline bool tartalmazza (const std::vector<Vektor>& v, Vektor p) {
  for (size_t i = 1; i<v.size(); ++i) {
    if (balra(v[i-1],v[i],p)) { return false; }
  }
  return (!balra(v[v.size()-1],v[0],p));
}
//Ket poligon metszi-e egymast (tfh a poligon teglalap xor haromszog)
inline bool intersect(const std::vector<Vektor>& v1, const std::vector<Vektor>& v2) {
  for (size_t i = 0; i<v1.size(); ++i) {
    if (tartalmazza(v2,v1[i])) { return true; }
  }
  for (size_t i = 0; i<v2.size(); ++i) {
    if (tartalmazza(v1,v2[i])) { return true; }
  }
  if (metszi(v1[0],v1[2],v2[0],v2[2]))                     { return true; }
  if (metszi(v1[0],v1[2],v2[1],v2[v2.size()-1]))           { return true; }
  if (metszi(v1[1],v1[v1.size()-1],v2[0],v2[2]))           { return true; }
  if (metszi(v1[1],v1[v1.size()-1],v2[1],v2[v2.size()-1])) { return true; }
  return false;
}
//egy lassabb, de biztosabb verzio
inline bool intersect2(const std::vector<Vektor>& v1, const std::vector<Vektor>& v2) {
  for (size_t i = 1; i<v1.size(); ++i) {
    for (size_t j = 1; j<v2.size(); ++j) {
      if (metszi(v1[i-1],v1[i],v2[j-1],v2[j])) { return true; }
    }
    if (metszi(v1[i-1],v1[i],v2[v2.size()-1],v2[0])) { return true; }
  }
  for (size_t j = 1; j<v2.size(); ++j) {
      if (metszi(v1[v1.size()-1],v1[0],v2[j-1],v2[j])) { return true; }
  }
  if (metszi(v1[v1.size()-1],v1[0],v2[v2.size()-1],v2[0])) { return true; }
  if (tartalmazza(v1,v2[0])) { return true; }
  return (tartalmazza(v2,v1[0]));
}
//a laser metszi-e a-b-t; tfh |irany|=1 es |a-b|>0
//t-re letezik p in [0..1]: pos+t*irany = p*a+(1-p)*b
inline double laserMetszi (Vektor pos, Vektor irany, Vektor a, Vektor b) {
  double fa = fordul(irany,a-pos), fb = fordul(irany,b-pos), t;
  if (std::abs(fa) < EPSILON && std::abs(fb) < EPSILON ) {
    if (rajta(a,pos+irany,pos) || rajta(b,pos+irany,pos)) {
      t = -1*INFTY;
    } else {
      t = std::min((b-pos).abs(),(a-pos).abs());
    }
  } else {
    if (fa*fb<=0) {
      double s = irany.y*(a.x-b.x)-irany.x*(a.y-b.y);
      if (s == 0) { s = EPSILON; }
      t = (pos.x*(a.y-b.y)-pos.y*(a.x-b.x)+a.x*b.y-a.y*b.x)/s;
    } else {
      t = -1*INFTY;
    }
  }
  return t;
}
//tukrozi irany-t az a-b egyenesrol
inline Vektor tukor (Vektor irany, Vektor a, Vektor b) {
  Vektor n {-1*(b-a).y,(b-a).x};
  Vektor m = n* ((irany*n)/n.abs()/n.abs());
  return irany - 2*m;
}
//elforgatja clockwise az iranyt phi radiannal
inline Vektor forgat (Vektor irany, double phi) {
  return {irany.x*std::cos(phi)-irany.y*std::sin(phi),
          irany.x*std::sin(phi)+irany.y*std::cos(phi)};
}


///Tesztelesek
inline bool isClockwise (const std::vector<Vektor>& v) {
  for (size_t i = 2; i<v.size(); ++i) {
    if (balra(v[i-2],v[i-1],v[i])) { return false; }
  }
  return ( (jobbra(v[v.size()-2],v[v.size()-1],v[0]))
        && (jobbra(v[v.size()-1],v[0],v[1])) );
}

inline std::vector<Vektor> generateHaromszog (Vektor pos, Vektor irany) {
  std::vector<Vektor> csucslista (3);
  csucslista[0] = pos;
  Vektor n {irany.y,-1*irany.x};
  csucslista[1] = pos - 10 * irany + 2.5 * n;
  csucslista[2] = pos - 10 * irany - 2.5 * n;
  return csucslista;
}

inline std::vector<Vektor> generateTeglalap (Vektor a, Vektor b) {
  std::vector<Vektor> csucslista (4);
  csucslista[0] = a;
  csucslista[1] = b;
  Vektor n {(b-a).y,-1*(b-a).x};
  csucslista[2] = b+n;
  csucslista[3] = a+n;
  return csucslista;
}










#endif //header guard







