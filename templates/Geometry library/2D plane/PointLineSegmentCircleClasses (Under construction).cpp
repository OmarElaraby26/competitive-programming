#define EPS 1e-9
#define Vector Point
#define _T_ long long // or long double

class Point {
public:
    _T_ x, y;

    Point() :
            x(0), y(0) {
    }

    Point(const _T_ a, const _T_ b) :
            x(a), y(b) {
    }

    Point(const Point &other) :
            Point(other.x, other.y) {
    }

    Point &operator=(const Point &other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    Point &operator-=(const Point &other) {
        Point(x - other.x, y - other.y);
        return *this;
    }

    Point &operator+=(const Point &other) {
        Point(x + other.x, y + other.y);
        return *this;
    }

    Point &operator*=(const Point &other) {
        Point(x * other.x, y * other.y);
        return *this;
    }

    Point operator+(const Point &other) const {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point &other) const {
        return Point(x - other.x, y - other.y);
    }

    Point operator*(const _T_ factor) const {
        return Point(x * factor, y * factor);
    }

    // factor must be long double
    Point operator/(const _T_ factor) const {
        assert(factor != 0);
        return Point(x / factor, y / factor);
    }

    Point operator-() const {
        return Point(-x, -y);
    }

    bool operator==(const Point &other) const {
        return (abs(x - other.x) < EPS && abs(y - other.y) < EPS);
    }

    bool operator!=(const Point &other) const {
        return !operator==(other);
    }

    bool operator<(const Point &other) const {
        if (abs(x - other.x) < EPS) { //equal
            if (abs(y - other.y) < EPS) return false;
            return y < other.y;
        }
        return x < other.x;
    }

    bool operator<=(const Point &other) const {
        return (operator==(other) || operator<(other));
    }

    bool operator>(const Point &other) const {
        if (abs(x - other.x) < EPS) { //equal
            if (abs(y - other.y) < EPS) return false;
            return y > other.y;
        }
        return x > other.x;
    }

    bool operator>=(const Point &other) const {
        return operator==(other) || operator>(other);
    }

    Point translate(const Vector &t) const {
        return this->operator+(t);
    }

    // scaling vector c->p by ratio
    Point scale(const Point &c, const double ratio) {
        // make vector c->p then scale this vector by ratio
        // then get the new point and return it
        // return new Point p' not new Vector cp'
        return (*this - c) * ratio + c;
    }

    Point scale(const double ratio) const {
        return operator*(ratio); // return new Vector v'
    }

    // rotate point by theta degree clockwise  around point c
    Point rotate(const Point &c, const double deg) const {
        double rad = deg / 180.0 * acos(-1);

        Vector vec(*this - c);

        vec = Vector(vec.x * cos(rad) - vec.y * sin(rad),
                     vec.y * cos(rad) + vec.x * sin(rad));

        return vec + c;
    }

    // clockwise
    Point rotate90(const Point &c) const {
        //	vec(x, y)->vec(y, -x);
        Vector vec(*this - c);
        return Point(vec.y, -vec.x) + c;
    }

    // clockwise
    Point rotate180(const Point &c) const {
        //		vec(x, y)->vec(-x, -y);
        Vector vec(*this - c);
        vec = -vec;
        return vec + c;
    }

    // clockwise
    Point rotate270(const Point &c) const {
        //		vec(x, y)->vec(-y, x);
        Vector vec(*this - c);
        return Point(-vec.y, vec.x) + c;
    }

    Point perp() const {
        return this->rotate270(Point(0, 0));
    }
};

Point make_point(const _T_ a, const _T_ b) {
    return Point(a, b);
}

ostream &operator<<(ostream &out, const Point &p) {
    return out << p.x << ' ' << p.y;
}

istream &operator>>(istream &in, Point &p) {
    return in >> p.x >> p.y;
}

_T_ crossProduct(const Vector &a, const Vector &b) {
    // |a|*|b|*sin(x)
    return (a.x * b.y - a.y * b.x);
}

_T_ dotProduct(const Vector &a, const Vector &b) {
    // |a|*|b|*cos(x)
    return (a.x * b.x + a.y * b.y);
}

// return positive value if c turn left
// return negative value if c turn right
// return ZERO if c on the same line(not necessary on the same segment)
_T_ orient(const Point &a, const Point &b, const Point &c) {
    return crossProduct(b - a, c - a);
}

// return true if Point 'testP' is in the angle (bac) or (cab)
// if on boundary then true
bool isInAngle(const Point &a, Point b, Point c, const Point &testP) {
    // in the following line, return true of abort ?
    assert(abs(orient(a, b, c)) >= EPS); // != 0
    if (orient(a, b, c) < 0) { // if true then swap
        const Point temp = b;
        b = c;
        c = temp;
    }
    const _T_ o1 = orient(a, b, testP);
    const _T_ o2 = orient(a, c, testP);
    return ((o1 > 0 || abs(o1) < EPS) && (o2 < 0 || abs(o2) < EPS));
}

// squared absolute value
_T_ sq(const Point &p) {
    return p.x * p.x + p.y * p.y;
}

long double abs(const Point &p) {
    return sqrtl(sq(p));
}

template<typename T>
int getSign(const T x) {
    return (x > T(0)) - (x < T(0));
}

class Line {
public:
    Vector v;
    _T_ c;

    Line(const Vector _v, const _T_ _c) :
            v(_v), c(_c) {
    }

    Line(_T_ _a, _T_ _b, _T_ _c) :
            v(_b, -_a), c(_c) {
    }

    Line(const Point &p, const Point &q) :
            v(q - p), c(crossProduct(v, p)) {
    }

    Line &operator=(const Line &l) {
        this->v = l.v;
        this->c = l.c;
        return *this;
    }

    // return 0 if point ON line
    // return > 0 if point on POSITIVE side
    // return < 0 if point on NEGATIVE side
    // positive side will be above the line if the line
    // from bottom-left to top-right
    _T_ side(const Point &p) const {
        return crossProduct(this->v, p) - this->c;
    }

    // shortest distance between the line and point 'p'
    long double distance(const Point &p) const {
        return abs(side(p)) / abs(v);
    }

    long double sqDistance(const Point &p) const {
        return side(p) * side(p) / (long double) sq(this->v);
    }

    // get line which is perp on the original one
    // and the point 'p' lies on the new one
    Line perpThrough(const Point &p) const {
        return Line(p, p + v.perp());
    }

    // sorting along the line
    bool cmpProj(const Point &p, const Point &q) const {
        return dotProduct(this->v, p) < dotProduct(this->v, q);
    }

    // translate the line by a vector 't'
    // only c will change. we should calculate the new c'
    Line translate(const Vector &t) const {
        return Line(v, c + crossProduct(v, t));
    }

    // same as translate.
    // except we translate by a vector perp to the line
    // translate to the left by 'dist'
    // or to the right by -'dist'
    Line shiftLeft(const double dist) const {
        return Line(v, c + dist * abs(v));
    }

    bool interSection(const Line &l2, Point &interP) const {
        const _T_ d = crossProduct(this->v, l2.v);

        if (abs(d) < EPS) return false;

        // interP -> intersection Point
        // requires long double
        interP = (l2.v * this->c - this->v * l2.c) / (long double) d;

        return true;
    }

    // return point 'p
    // that is the point p after dropped on the line
    Point project(const Point &p) const {
        return p - this->v.perp() * this->side(p) / sq(v);
    }

    Point reflect(const Point &p) const {
        return p - (this->v.perp() * 2.0 * this->side(p) / sq(this->v));
    }

    Line bisector(const Line &l2, const bool interior) const {
        //  two lines can not be parallel
        assert(abs(crossProduct(this->v, l2.v)) >= EPS);

        // interior == true means line between smallest angle
        double sign = interior ? 1 : -1;

        return Line(l2.v / abs(l2.v) + this->v / abs(this->v) * sign,
                    l2.c / abs(l2.v) + this->c / abs(this->v) * sign);
    }

};

//struct cmpSegSegInter {
//	bool operator ()(const Point &a, const Point &b) const {
//		return a < b;
//	}
//};

class Segment {
public:
    Point a, b;

    Segment() {
    }

    Segment(const Point _a, const Point _b) :
            a(_a), b(_b) {
    }

    Segment(const Segment &other) :
            Segment(other.a, other.b) {

    }

    Segment &operator=(const Segment &other) {
        this->a = other.a;
        this->b = other.b;
        return *this;
    }

    bool operator==(const Segment &other) const {
        return (a == other.a && b == other.b);
    }

    // Is testP is in disk of segment a-b ?
    bool isInDisk(const Point &testP) const {
        const _T_ res = dotProduct(a - testP, b - testP);
        return (res < 0 || abs(res) < EPS); // res <= 0
    }

    bool isOnSegment(const Point &testP) const {
        return (abs(orient(a, b, testP)) < EPS && this->isInDisk(testP));
    }

    bool ProperInter(const Segment &seg, Point &InterP) const {
        const Point &c = seg.a, &d = seg.b;
        const double oa = orient(c, d, a);
        const double ob = orient(c, d, b);
        const double oc = orient(a, b, c);
        const double od = orient(a, b, d);

        // should take care of precision error ?
        // use EPS in the following conditions ?
        if (oa * ob < 0 && oc * od < 0) {
            InterP = (a * ob - b * oa) / (ob - oa);
            return true;
        }

        return false;
    }

    // return intersection between seg a-b & seg c-d
    // if size of returned set is 1 then the intersection is point
    // else if 2 then the intersection is segment
    // else (0) the no intersection
    set<Point> segSegInter(const Segment &seg) const {
        Point out;
        if (this->ProperInter(seg, out)) return {out};

        const Point &c = seg.a, &d = seg.b;

        set<Point> s;
        if (seg.isOnSegment(a)) s.insert(a);
        if (seg.isOnSegment(b)) s.insert(b);
        if (this->isOnSegment(c)) s.insert(c);
        if (this->isOnSegment(d)) s.insert(d);

        return s;
    }

    // return shortest distance from point p to seg a-b
    long double segPointDistance(const Point &p) const {
        if (a != b) {
            const Line l(a, b);
            if (l.cmpProj(a, p) && l.cmpProj(p, b)) {
                return l.distance(p);
            }
        }

        return min(abs(p - a), abs(p - b));
    }

    // return shortest distance between seg a-b and seg c-d
    long double segSegDistance(const Segment &seg) const {
        Point dummy;
        if (this->ProperInter(seg, dummy)) return 0;

        const Point &c = seg.a, &d = seg.b;
        return min(
                {segPointDistance(c), segPointDistance(d),
                 seg.segPointDistance(a), seg.segPointDistance(b)});
    }

};


long double PolyArea(const vector<Point> &poly) { // O(pts.size())
    long double res = 0.0;
    for (int i = 0, n = poly.size(); i < n; ++i) {
        res += crossProduct(poly[i], poly[(i + 1 < n) ? i + 1 : 0]);
    }
    return abs(res) / 2.0;
}

bool isConvex(const vector<Point> &poly) { // O(pts.size())
    bool hasPos = false, hasNeg = false;
    for (int i = 0, n = poly.size(); i < n; ++i) {
        int o = orient(poly[i], poly[(i + 1) % n], poly[(i + 2) % n]);
        if (o > 0) hasPos = true;
        if (o < 0) hasNeg = true;
    }
    return !(hasPos && hasNeg);
}


void convex_hull(vector<Point> &a, const bool include_collinear = false) {
    // delete duplicated points
    sort(a.begin(), a.end());
    a.resize(unique(a.begin(), a.end()) - a.begin());

    const Point p0 = *min_element(a.begin(), a.end(), [](Point a, Point b) {
        if (abs(a.y - b.y) < EPS) {
            if (abs(a.x - b.x) < EPS) return false;
            return a.x < b.x;
        }
        return a.y < b.y;
    });

    sort(a.begin(), a.end(), [p0](const Point &a, const Point &b) {
        const _T_ o = orient(p0, a, b);
        if (abs(o) < EPS) { // == 0
            const _T_ val = (p0.x - a.x) * (p0.x - a.x) + (p0.y - a.y) * (p0.y - a.y)
                            - (p0.x - b.x) * (p0.x - b.x) - (p0.y - b.y) * (p0.y - b.y);
            return (abs(val) > EPS) && val < 0;
        }
        return o < 0;
    });

    const auto collinear = [](Point a, Point b, Point c) -> bool {
        return abs(orient(a, b, c)) < EPS;
    };

    if (include_collinear) {
        int i = int(a.size()) - 1;
        while (i >= 0 && collinear(p0, a[i], a.back())) --i;
        reverse(a.begin() + i + 1, a.end());
    }

    const auto cw = [include_collinear](Point a, Point b, Point c) -> bool {
        const _T_ o = orient(a, b, c);
        return (abs(o) > EPS && o < 0) || (include_collinear && abs(o) < EPS);
    };

    vector<Point> st;
    for (int i = 0; i < (int) a.size(); ++i) {
        while (st.size() > 1 && !cw(st[st.size() - 2], st.back(), a[i]))
            st.pop_back();
        st.push_back(a[i]);
    }

    a = st;
}

// todo: check if point inside the polygon O(log(n))
// todo: sort points of polygon clockwise

// Any 3 points that are NOT on the same line
// can be on a circle
// circumCenter() return the center of this circle
Point circumCenter(Point a, Point b, Point c) {
    b -= a; // consider coordinates relative to A
    c -= a;

    assert(abs(crossProduct(b, c)) >= EPS); // != 0

    return a + (b * sq(c) - c * sq(b)).perp() / crossProduct(b, c) / 2.0;
}

// return the number of intersections and places them in 'out'.
int circleLineInter(const Point &o, const long double r, const Line &l,
                    pair<Point, Point> &out) {
    long double h2 = r * r - l.sqDistance(o);
    if (h2 > 0 || abs(h2) < EPS) { // >= 0
        if (h2 < 0) h2 = 0; // to avoid precision error
        const Point p = l.project(o);
        const Point h = l.v * sqrtl(h2) / abs(l.v);
        out = {p - h, h + h};
    }

    return 1 + getSign(h2);
}

// return the number of intersections (0, 1, 2) and places them in 'out'.
// if two circles are identical then there are infinite number of points
// so it will return 3
int circleCircleInter(const Point &o1, const long double r1, const Point &o2,
                      const long double r2, pair<Point, Point> &out) {
    const Point d = o2 - o1;
    const long double d2 = sq(d);
    if (abs(d2) < EPS) { // concentric circles
        if (abs(r1 - r2) < EPS) return 3; // identical circles
        return 0;
    }
    const long double pd = (d2 + r1 * r1 - r2 * r2) / 2; // = |O_1P| * d
    const long double h2 = r1 * r1 - pd * pd / d2; // = hˆ2
    if (h2 >= 0) {
        const Point p = o1 + d * pd / d2, h = d.perp() * sqrtl(h2 / d2);
        out = {p - h, p + h};
    }
    return 1 + getSign(h2);
}


int tangents(const Point &o1, const long double r1, const Point &o2,
             long double r2, const bool inner, vector<pair<Point, Point> > &out) {
    if (inner) r2 = -r2;
    const Point d = o2 - o1;
    long double dr = r1 - r2, d2 = sq(d), h2 = d2 - dr * dr;
    if (abs(d2) < EPS || h2 < 0) {
        assert(h2 != 0);
        return 0;
    }
    for (const long double sign : {-1, 1}) {
        Point v = (d * dr + d.perp() * sqrt(h2) * sign) / d2;
        out.push_back({o1 + v * r1, o2 + v * r2});
    }
    return 1 + (h2 > 0);
}
