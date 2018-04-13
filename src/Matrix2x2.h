// Original source code by Kenshi Takayama

template <typename Scalar>
class Matrix<Scalar, 2, 2> {
public:
    typedef Scalar value_type;
    typedef Matrix<Scalar, 2, 2> matrix_type;
private:
    value_type values_[4];
public:
    // constructors ===============
    Matrix() { fill(0); }
    Matrix(const matrix_type& src) { operator=(src); }
    template<typename OtherScalar>
    explicit Matrix(const Matrix<OtherScalar, 2, 2>& src) { operator=(src); }
    explicit Matrix(const value_type& s) { fill(s); }
    explicit Matrix(const value_type v[4]) { memcpy(values_, v, 4 * sizeof(value_type)); }
    explicit Matrix(const Vector<Scalar, 2>& diagonal) { setDiagonal(diagonal); }
    explicit Matrix(const Vector<Scalar, 2>& vecRow, const Vector<Scalar, 2>& vecCol) { setTensorProd(vecRow, vecCol); }
    ~Matrix() {}
    
    // dimension-wise constructors =================
    Matrix(
        const value_type& s00, const value_type& s01,
        const value_type& s10, const value_type& s11)
    {
        set(
            s00, s01,
            s10, s11);
    }
    // dimension-wise setters =================
    void set(
        const value_type& s00, const value_type& s01,
        const value_type& s10, const value_type& s11)
    {
        assert(2 == 2 && 2 == 2);
        values_[0] = s00;    values_[1] = s01;
        values_[2] = s10;    values_[3] = s11;
    }
    void setTensorProd(const Vector<Scalar, 2>& vecRow, const Vector<Scalar, 2>& vecCol) {
        for (size_t i = 0; i < 2; ++i) for (size_t j = 0; j < 2; ++j) values_[2 * i + j] = vecRow[j] * vecCol[i];
    }
    void setDiagonal(const Vector<Scalar, 2>& diagonal) {
        set(
            diagonal[0], 0, 
            0, diagonal[1]);
    }
    void setIdentity() {
        set(
            1, 0,
            0, 1);
    }
    void setRow(size_t rowIndex, const Vector<Scalar, 2>& rowValue) {
        memcpy(&values_[2 * rowIndex], rowValue.ptr(), 2 * sizeof(value_type));
    }
    void setCol(size_t colIndex, const Vector<Scalar, 2>& colValue) {
        for (size_t row = 0; row < 2; ++row) (*this)(row, colIndex) = colValue[row];
    }
    Vector<Scalar, 2> getRow(size_t rowIndex) const {
        Vector<Scalar, 2> result;
        memcpy(result.ptr(), &values_[2 * rowIndex], 2 * sizeof(value_type));
        return result;
    }
    Vector<Scalar, 2> getCol(size_t colIndex) const {
        Vector<Scalar, 2> result;
        for (size_t row = 0; row < 2; ++row) result[row] = (*this)(row, colIndex);
        return result;
    }
    void setRows(
        const Vector<Scalar, 2>& rowValue0,
        const Vector<Scalar, 2>& rowValue1)
    {
        set(
            rowValue0[0], rowValue0[1],
            rowValue1[0], rowValue1[1]
        );
    }
    void setCols(
        const Vector<Scalar, 2>& colValue0,
        const Vector<Scalar, 2>& colValue1)
    {
        set(
            colValue0[0], colValue1[0],
            colValue0[1], colValue1[1]
        );
    }
    
    // substitution operators ===================
    matrix_type& operator=(const matrix_type& src) {
        if (this != &src) memcpy(values_, src.values_, 4 * sizeof(value_type));
        return *this;
    }
    template<typename OtherScalar>
    matrix_type& operator=(const Matrix<OtherScalar, 2, 2>& src) {
        for (size_t i = 0; i < 4; ++i) values_[i] = static_cast<value_type>(src.values_[i]);
        return *this;
    }
    
    // fill ====================
    void fill(const value_type& s) { std::fill(values_, values_ + 4, s); }
    
    // cast to raw pointer ====================
    value_type* ptr() { return values_; }
    const value_type* ptr() const { return values_; }
    
    // indexing ==========================
    value_type& operator[](size_t index) { return values_[index]; }
    const value_type& operator[](size_t index) const { return values_[index]; }
    value_type& operator()(size_t row, size_t col) { return values_[2 * row + col]; }
    const value_type& operator()(size_t row, size_t col) const { return values_[2 * row + col]; }
    
    // comparison ======================
    bool operator==(const matrix_type& rhs) const {
        for (size_t i = 0; i < 4; ++i) if (values_[i] != rhs.values_[i]) return false;
        return true;
    }
    bool operator!=(const matrix_type& rhs) const { return !operator==(rhs); }

    // arithmetic (member) =====================
    // with scalar
    matrix_type& operator+=(const value_type& rhs) { for (size_t i = 0; i < 4; ++i) values_[i] += rhs; return *this; }
    matrix_type& operator-=(const value_type& rhs) { for (size_t i = 0; i < 4; ++i) values_[i] -= rhs; return *this; }
    matrix_type& operator*=(const value_type& rhs) { for (size_t i = 0; i < 4; ++i) values_[i] *= rhs; return *this; }
    matrix_type& operator/=(const value_type& rhs) { for (size_t i = 0; i < 4; ++i) values_[i] /= rhs; return *this; }
    matrix_type operator+(const value_type& rhs) const { return matrix_type(*this) += rhs; }
    matrix_type operator-(const value_type& rhs) const { return matrix_type(*this) -= rhs; }
    matrix_type operator*(const value_type& rhs) const { return matrix_type(*this) *= rhs; }
    matrix_type operator/(const value_type& rhs) const { return matrix_type(*this) /= rhs; }
    // with same sized matrix
    matrix_type& operator+=(const matrix_type& rhs) { for (size_t i = 0; i < 4; ++i) values_[i] += rhs.values_[i]; return *this; }
    matrix_type& operator-=(const matrix_type& rhs) { for (size_t i = 0; i < 4; ++i) values_[i] -= rhs.values_[i]; return *this; }
    matrix_type operator+(const matrix_type& rhs) const { return matrix_type(*this) += rhs; }
    matrix_type operator-(const matrix_type& rhs) const { return matrix_type(*this) -= rhs; }
    // mult with vector
    Vector<Scalar, 2> operator*(const Vector<Scalar, 2>& rhs) const {
        Vector<Scalar, 2> result;
        for (size_t row = 0; row < 2; ++row) for (size_t col = 0; col < 2; ++col) result[row] += (*this)(row, col) * rhs[col];
        return result;
    }
    // mult with matrix
    template<int NColNew>
    Matrix<Scalar, 2, NColNew> operator*(const Matrix<Scalar, 2, NColNew>& rhs) { 
        Matrix<Scalar, 2, NColNew> result;
        for (size_t row = 0; row < 2; ++row) for (size_t col = 0; col < NColNew; ++col) {
            Scalar sum = 0;
            for (size_t k = 0; k < 2; ++k) sum += (*this)(row, k) * rhs(k, col);
            result(row, col) = sum;
        }
        return result;
    }
    // negate
    matrix_type operator-() const {
        matrix_type result;
        for (size_t i = 0; i < 4; ++i) result.values_[i] = -values_[i];
        return result;
    }
    
    // matrix operations ==================
    // transpose
    matrix_type transpose() const {
        matrix_type result;
        for (size_t row = 0; row < 2; ++row) for (size_t col = 0; col < 2; ++col) result(col, row) = operator()(row, col);
        return result;
    }
    value_type determinant() const {
        const matrix_type& m = *this;
        return m(0,0)*m(1,1) - m(0,1)*m(1,0);
    }
    bool invert() {
        Scalar s = determinant();
        if (s == 0.0) return false;
        s = 1/s;
        matrix_type m(*this);
        this->set(
             m(1,1), -m(0,1),
            -m(1,0),  m(0,0));
        (*this) *= s;
        return true;
    }
    bool solve(const Vector<Scalar, 2>& b, Vector<Scalar, 2>& x) const {
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
