// Original source code by Kenshi Takayama

template <typename Scalar>
class Matrix<Scalar, 3, 3> {
public:
    typedef Scalar value_type;
    typedef Matrix<Scalar, 3, 3> matrix_type;
    typedef Vector<Scalar, 3   > vector_type;
    
    // modified by Yuki Koyama
    union {
        value_type values_[9];
        value_type m[3][3];
    };

    // constructors ===============
    Matrix() { fill(0); }
    Matrix(const matrix_type& src) { operator=(src); }
    template<typename OtherScalar>
    explicit Matrix(const Matrix<OtherScalar, 3, 3>& src) { operator=(src); }
    explicit Matrix(const value_type& s) { fill(s); }
    explicit Matrix(const value_type v[9]) { memcpy(values_, v, 9 * sizeof(value_type)); }
    explicit Matrix(const Vector<Scalar, 3>& diagonal) { setDiagonal(diagonal); }
    explicit Matrix(const Vector<Scalar, 3>& vecRow, const Vector<Scalar, 3>& vecCol) { setTensorProd(vecRow, vecCol); }
    ~Matrix() {}
    
    // dimension-wise constructors =================
    Matrix(
        const value_type& s00, const value_type& s01, const value_type& s02,
        const value_type& s10, const value_type& s11, const value_type& s12,
        const value_type& s20, const value_type& s21, const value_type& s22)
    {
        set(
            s00, s01, s02,
            s10, s11, s12,
            s20, s21, s22);
    }

    // dimension-wise setters =================
    void set(
        const value_type& s00, const value_type& s01, const value_type& s02,
        const value_type& s10, const value_type& s11, const value_type& s12,
        const value_type& s20, const value_type& s21, const value_type& s22)
    {
        values_[0] = s00;    values_[1] = s01;    values_[2] = s02;
        values_[3] = s10;    values_[4] = s11;    values_[5] = s12;
        values_[6] = s20;    values_[7] = s21;    values_[8] = s22;
    }
    void setTensorProd(const Vector<Scalar, 3>& vecRow, const Vector<Scalar, 3>& vecCol) {
        for (size_t i = 0; i < 3; ++i) for (size_t j = 0; j < 3; ++j) values_[3 * i + j] = vecRow[j] * vecCol[i];
    }
    void setDiagonal(const Vector<Scalar, 3>& diagonal) {
        set(
            diagonal[0], 0, 0,
            0, diagonal[1], 0,
            0, 0, diagonal[2]);
    }
    void setIdentity() {
        set(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1);
    }
    void setRow(size_t rowIndex, const Vector<Scalar, 3>& rowValue) {
        memcpy(&values_[3 * rowIndex], rowValue.ptr(), 3 * sizeof(value_type));
    }
    void setCol(size_t colIndex, const Vector<Scalar, 3>& colValue) {
        for (size_t row = 0; row < 3; ++row) (*this)(row, colIndex) = colValue[row];
    }
    Vector<Scalar, 3> getRow(size_t rowIndex) const {
        Vector<Scalar, 3> result;
        memcpy(result.ptr(), &values_[3 * rowIndex], 3 * sizeof(value_type));
        return result;
    }
    Vector<Scalar, 3> getCol(size_t colIndex) const {
        Vector<Scalar, 3> result;
        for (size_t row = 0; row < 3; ++row) result[row] = (*this)(row, colIndex);
        return result;
    }
    void setRows(
        const Vector<Scalar, 3>& rowValue0,
        const Vector<Scalar, 3>& rowValue1,
        const Vector<Scalar, 3>& rowValue2)
    {
        set(
            rowValue0[0], rowValue0[1], rowValue0[2],
            rowValue1[0], rowValue1[1], rowValue1[2],
            rowValue2[0], rowValue2[1], rowValue2[2]);
    }
    void setCols(
        const Vector<Scalar, 3>& colValue0,
        const Vector<Scalar, 3>& colValue1,
        const Vector<Scalar, 3>& colValue2)
    {
        set(
            colValue0[0], colValue1[0], colValue2[0],
            colValue0[1], colValue1[1], colValue2[1],
            colValue0[2], colValue1[2], colValue2[2]);
    }
    
    // substitution operators ===================
    matrix_type& operator=(const matrix_type& src) {
        if (this != &src) memcpy(values_, src.values_, 9 * sizeof(value_type));
        return *this;
    }
    template<typename OtherScalar>
    matrix_type& operator=(const Matrix<OtherScalar, 3, 3>& src) {
        for (size_t i = 0; i < 9; ++i) values_[i] = static_cast<value_type>(src.values_[i]);
        return *this;
    }
    
    // fill ====================
    void fill(const value_type& s) { std::fill(values_, values_ + 9, s); }
    
    // cast to raw pointer ====================
    value_type* ptr() { return values_; }
    const value_type* ptr() const { return values_; }
    
    // indexing ==========================
    value_type& operator[](size_t index) { return values_[index]; }
    const value_type& operator[](size_t index) const { return values_[index]; }
    value_type& operator()(size_t row, size_t col) { return values_[3 * row + col]; }
    const value_type& operator()(size_t row, size_t col) const { return values_[3 * row + col]; }
    
    // comparison ======================
    bool operator==(const matrix_type& rhs) const {
        for (size_t i = 0; i < 9; ++i) if (values_[i] != rhs.values_[i]) return false;
        return true;
    }
    bool operator!=(const matrix_type& rhs) const { return !operator==(rhs); }

    // arithmetic (member) =====================
    // with scalar
    matrix_type& operator+=(const value_type& rhs) { for (size_t i = 0; i < 9; ++i) values_[i] += rhs; return *this; }
    matrix_type& operator-=(const value_type& rhs) { for (size_t i = 0; i < 9; ++i) values_[i] -= rhs; return *this; }
    matrix_type& operator*=(const value_type& rhs) { for (size_t i = 0; i < 9; ++i) values_[i] *= rhs; return *this; }
    matrix_type& operator/=(const value_type& rhs) { for (size_t i = 0; i < 9; ++i) values_[i] /= rhs; return *this; }
    matrix_type operator+(const value_type& rhs) const { return matrix_type(*this) += rhs; }
    matrix_type operator-(const value_type& rhs) const { return matrix_type(*this) -= rhs; }
    matrix_type operator*(const value_type& rhs) const { return matrix_type(*this) *= rhs; }
    matrix_type operator/(const value_type& rhs) const { return matrix_type(*this) /= rhs; }
    // with same sized matrix
    matrix_type& operator+=(const matrix_type& rhs) { for (size_t i = 0; i < 9; ++i) values_[i] += rhs.values_[i]; return *this; }
    matrix_type& operator-=(const matrix_type& rhs) { for (size_t i = 0; i < 9; ++i) values_[i] -= rhs.values_[i]; return *this; }
    matrix_type operator+(const matrix_type& rhs) const { return matrix_type(*this) += rhs; }
    matrix_type operator-(const matrix_type& rhs) const { return matrix_type(*this) -= rhs; }
    // mult with vector
    Vector<Scalar, 3> operator*(const Vector<Scalar, 3>& rhs) const {
        Vector<Scalar, 3> result;
        for (size_t row = 0; row < 3; ++row) for (size_t col = 0; col < 3; ++col) result[row] += (*this)(row, col) * rhs[col];
        return result;
    }
    // mult with matrix
    template<int NColNew>
    Matrix<Scalar, 3, NColNew> operator*(const Matrix<Scalar, 3, NColNew>& rhs) { 
        Matrix<Scalar, 3, NColNew> result;
        for (size_t row = 0; row < 3; ++row) for (size_t col = 0; col < NColNew; ++col) {
            Scalar sum = 0;
            for (size_t k = 0; k < 3; ++k) sum += (*this)(row, k) * rhs(k, col);
            result(row, col) = sum;
        }
        return result;
    }
    // negate
    matrix_type operator-() const {
        matrix_type result;
        for (size_t i = 0; i < 9; ++i) result.values_[i] = -values_[i];
        return result;
    }
    
    // matrix operations ==================
    // transpose
    matrix_type transpose() const {
        matrix_type result;
        for (size_t row = 0; row < 3; ++row) for (size_t col = 0; col < 3; ++col) result(col, row) = operator()(row, col);
        return result;
    }
    value_type determinant() const {
        const matrix_type& m = *this;
        return
             m(0,0)*(m(1,1)*m(2,2) - m(2,1)*m(1,2))
            -m(0,1)*(m(1,0)*m(2,2) - m(2,0)*m(1,2))
            +m(0,2)*(m(1,0)*m(2,1) - m(2,0)*m(1,1));
    }
    bool invert() {
        Scalar s = determinant();
        if (s == 0.0) return false;
        s = 1/s;
        matrix_type m(*this);
        this->set(
            m(1,1)*m(2,2) - m(1,2)*m(2,1), m(0,2)*m(2,1) - m(0,1)*m(2,2), m(0,1)*m(1,2) - m(0,2)*m(1,1),
            m(1,2)*m(2,0) - m(1,0)*m(2,2), m(0,0)*m(2,2) - m(0,2)*m(2,0), m(0,2)*m(1,0) - m(0,0)*m(1,2),
            m(1,0)*m(2,1) - m(1,1)*m(2,0), m(0,1)*m(2,0) - m(0,0)*m(2,1), m(0,0)*m(1,1) - m(0,1)*m(1,0));
        (*this) *= s;
        return true;
    }
    matrix_type inverse() const {
        Scalar s = determinant();
        if (s == 0.0) {
            printf("error: fail to Matrix::inverse()\n");
            return identity();   
        }
        s = 1.0 / s;
        matrix_type m(*this);
        matrix_type m2;

        m2.set(
                  m(1,1)*m(2,2) - m(1,2)*m(2,1), m(0,2)*m(2,1) - m(0,1)*m(2,2), m(0,1)*m(1,2) - m(0,2)*m(1,1),
                  m(1,2)*m(2,0) - m(1,0)*m(2,2), m(0,0)*m(2,2) - m(0,2)*m(2,0), m(0,2)*m(1,0) - m(0,0)*m(1,2),
                  m(1,0)*m(2,1) - m(1,1)*m(2,0), m(0,1)*m(2,0) - m(0,0)*m(2,1), m(0,0)*m(1,1) - m(0,1)*m(1,0));
        m2 *= s;
        return m2;
    }
    bool solve(const Vector<Scalar, 3>& b, Vector<Scalar, 3>& x) const {
        matrix_type inv(*this);
        if (!inv.invert()) return false;
        x = inv * b;
        return true;
    }
    // static maker ==========================
    static matrix_type identity() {
        matrix_type result;
        result.setIdentity();
        return result;
    }
};
