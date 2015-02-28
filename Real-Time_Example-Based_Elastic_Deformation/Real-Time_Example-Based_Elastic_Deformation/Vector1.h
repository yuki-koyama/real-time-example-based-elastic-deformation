// Original source code by Kenshi Takayama

// special case of 1D vector (considered as convertible to scalar) ===========================-
template <typename Scalar>
class Vector<Scalar, 1> {
public:
    typedef Scalar value_type;
    typedef Vector<Scalar, 1> vector_type;
    value_type value_;
    
    static const size_t size_ = 1;  // OpenMesh compatible
    
    // constructor ===============
    Vector() :value_(0) {}
    // copy constructor and substitution operator are default
    /*explicit*/ Vector(const value_type& s) : value_(s) {}
    ~Vector() {}
    
    // implicit conversion to Scalar =====================
    operator Scalar() const { return value_; }
    
    // setter =================
    void set(const value_type& s) { value_ = s; }
    // type conversion ==========================
    template<typename OtherScalar>
    Vector<OtherScalar, 1> convert() const { return Vector<OtherScalar, 1>(static_cast<OtherScalar>(value_)); }
    // comparison ===============================
    bool operator==(const vector_type& rhs) const { return equals(rhs); }
    bool operator!=(const vector_type& rhs) const { return !equals(rhs); }
    bool equals(const vector_type& rhs, Scalar epsilon = std::numeric_limits<Scalar>::epsilon()) const {
        value_type d = value_ - rhs.value_;
        if (d <= -epsilon || epsilon <= d) return false;
        return true;
    }
    // fill ====================
    void fill(const value_type& s) { value_ = s; }
    // cast to raw pointer ====================
    value_type* ptr() { return &value_; }
    const value_type* ptr() const { return &value_; }
    // indexing ==========================
    value_type& operator[](size_t index) { assert(index == 0); return value_; }
    const value_type& operator[](size_t index) const { assert(index == 0); return value_; }
    // arithmetic (member) =====================
    // with scalar
    vector_type& operator+=(const value_type& rhs) { value_ += rhs; return *this; }
    vector_type& operator-=(const value_type& rhs) { value_ -= rhs; return *this; }
    vector_type& operator*=(const value_type& rhs) { value_ *= rhs; return *this; }
    vector_type& operator/=(const value_type& rhs) { value_ /= rhs; return *this; }
    vector_type operator+(const value_type& rhs) const { return vector_type(*this) += rhs; }
    vector_type operator-(const value_type& rhs) const { return vector_type(*this) -= rhs; }
    vector_type operator*(const value_type& rhs) const { return vector_type(*this) *= rhs; }
    vector_type operator/(const value_type& rhs) const { return vector_type(*this) /= rhs; }
    // negate
    vector_type operator-() const { return vector_type(-value_); }
    
    // vector operation =========================
    // dot product
    value_type operator|(const vector_type& rhs) const { return value_ * rhs.value_; }
    value_type dot_product(const vector_type& rhs) const { return (*this) | (rhs); }
    // normalize
    value_type norm() const { return static_cast<value_type>(std::abs(value_)); }
    value_type sqnorm() const { return (*this) | (*this); }
    value_type length() const { return norm(); }
    value_type lengthSquared() const { return sqnorm(); }
    vector_type normalized() const { return (*this) / norm(); }
    void normalize()  { (*this) /= norm(); }
    void normalize_cond() {
        value_type r = norm();
        (*this) /= (r == static_cast<value_type>(0)) ? 1 : r;
    }
    
    // max / min / sum / mean ==========================
    value_type elemMin() const { return value_; }
    value_type elemMax() const { return value_; }
    value_type elemSum() const { return value_; }
    value_type elemMean() const {  return value_; }
    vector_type pairwiseMax(const vector_type& rhs) const { return vector_type(value_ < rhs.value_ ? rhs.value_ : value_); }
    vector_type pairwiseMin(const vector_type& rhs) const { return vector_type(value_ > rhs.value_ ? rhs.value_ : value_); }
    void set_random(value_type range_min, value_type range_max) {
        value_type range_size = range_max - range_min;
        value_type val = static_cast<value_type>(rand() * range_size / RAND_MAX);
        value_ = range_min + val;
    }
    // append ====================
    template <int M>
    Vector<Scalar, 1 + M> append(const Vector<Scalar, M>& tail) const {
        Vector<Scalar, 1 + M> result;
        result[0] = value_;
        for (size_t i = 0; i < M; ++i)
            result[1 + i] = tail[i];
        return result;
    }
};
