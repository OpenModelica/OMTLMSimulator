#ifndef _FLOAT33S_ICC
#define _FLOAT33S_ICC

//! Functions for  float33s



inline float33s::float33s(const float a11, const float a22, const float a33,
                          const float a12, const float a23, const float a13) :
                x11(a11),
                x22(a22),
                x33(a33),
                x12(a12),
                x23(a23),
                x13(a13){}


//! Constructor that initializes the components to the given value.
inline float33s::float33s(const float val) :
                x11(val),
                x22(val),
                x33(val),
                x12(val),
                x23(val),
                x13(val){}

inline const float33s operator+(const float33s& a, const float33s& b){
    return float33s(a.x11+b.x11,a.x22+b.x22,a.x33+b.x33,a.x12+b.x12,a.x23+b.x23,a.x13+b.x13);
}

inline const float33s operator-(const float33s& a, const float33s& b){
    return float33s(a.x11-b.x11,a.x22-b.x22,a.x33-b.x33,a.x12-b.x12,a.x23-b.x23,a.x13-b.x13);
}

inline bool operator==(const float33s& a, const float33s& b) {
    return ((a.x11 == b.x11) && (a.x22 == b.x22) && (a.x33 == b.x33) &&
            (a.x12 == b.x12) && (a.x23 == b.x23) && (a.x13 == b.x13));
}

//! M != M (no fuzz for comparison)
inline bool operator!=(const float33s& a, const float33s& b) {
    return !(a==b);
}

inline float33s& float33s::operator=(const float33s& m) {
    x11 = m.x11;
    x22 = m.x22;
    x33 = m.x33;
    x12 = m.x12;
    x23 = m.x23;
    x13 = m.x13;
    return (*this);
}

//! Conversion to string.
Bstring ToStr(const float33s& val);

inline float33s& float33s::operator+=(const float33s& m) {
    x11 += m.x11;
    x22 += m.x22;
    x33 += m.x33;
    x12 += m.x12;
    x23 += m.x23;
    x13 += m.x13;
    return (*this);
}

inline float33s& float33s::operator-=(const float33s& m) {
    x11 -= m.x11;
    x22 -= m.x22;
    x33 -= m.x33;
    x12 -= m.x12;
    x23 -= m.x23;
    x13 -= m.x13;
    return (*this);
}





inline float& float33s::operator()(const int row, const int col) {
        switch(row) {
        case 1:
            switch(col) {
            case 1: return x11;
            case 2: return x12;
            case 3: return x13;
            default:assert(false);
            }
        case 2:
            switch(col) {
            case 1: return x12;
            case 2: return x22;
            case 3: return x23;
            default:assert(false);
            }
        case 3:
            switch(col) {
            case 1: return x13;
            case 2: return x23;
            case 3: return x33;
            default:assert(false);
            }
        default:assert(false);
        }
        assert(0);
        return x11;
}


inline float float33s::operator()(const int row, const int col) const {
        switch(row) {
        case 1:
            switch(col) {
            case 1: return x11;
            case 2: return x12;
            case 3: return x13;
            default:assert(false);
            }
        case 2:
            switch(col) {
            case 1: return x12;
            case 2: return x22;
            case 3: return x23;
            default:assert(false);
            }
        case 3:
            switch(col) {
            case 1: return x13;
            case 2: return x23;
            case 3: return x33;
            default:assert(false);
            }
        default:assert(false);
        }
        assert(0);
        return x11;
}

inline bool float33s::isZero() const
{
    return (x11==0.0)&&(x22==0.0)&&(x33==0.0)&&(x12==0.0)&&(x23==0.0)&&(x13==0.0);
}

inline bool float33s::isNonZero() const
{
    return (x11!=0.0)||(x22!=0.0)||(x33!=0.0)||(x12!=0.0)||(x23!=0.0)||(x13!=0.0);
}

//! Compute effective (von Misses) stress
inline float float33s::calc_vonMises() const {
    float tmp = x11*(x11 - x22) + x22*(x22 - x33) + x33*(x33 - x11)
            + 3*(x12*x12 + x23*x23 + x13*x13);
    // Due to numerical error, we can get small negative values.
    return (tmp > 0) ? sqrt(tmp):0;
}

#endif
