#include <iostream>
#include <utility>
#include <cstddef>
#include <limits>

class Ints
{
public:
  Ints();
  Ints(size_t size, int value);
  Ints(const Ints & rhs);
  Ints(Ints && rhs) noexcept;
  Ints & operator=(const Ints & rhs);
  Ints & operator=(Ints && rhs);
  Ints operator+(const Ints & rhs) const;
  ~Ints();

  size_t getSize() const noexcept;
  void set(size_t id, int i) noexcept;
  int get(size_t id) const noexcept;
  void setSafe(size_t id, int i);
  int getSafe(size_t id) const;

  Ints append(int i);
  Ints append(Ints b);

  static Ints unitializatedInts(size_t size);

private:
  void resize();
  size_t capacity;
  size_t size;
  int * data;
};

Ints::Ints():
  capacity(0),
  size(0),
  data(nullptr)
{}

Ints::Ints(size_t size, int value):
  capacity(size),
  size(size),
  data(new int [size])
{
  for (size_t i = 0; i < size; ++i)
  {
    data[i] = value;
  }
}

Ints Ints::unitializatedInts(size_t size)
{
  Ints temp;
  temp.size = 0;
  temp.capacity = size;
  temp.data = new int [size];

  return temp;
}

Ints::Ints(const Ints & rhs):
  capacity(rhs.capacity),
  size(rhs.size),
  data(new int [size])
{
  for (size_t i = 0; i < size; ++i)
  {
    data[i] = rhs.data[i];
  }
}

Ints::Ints(Ints && rhs) noexcept:
  capacity(rhs.capacity),
  size(rhs.size),
  data(rhs.data)
{
  rhs.capacity = 0;
  rhs.size = 0;
  rhs.data = nullptr;
}

Ints & Ints::operator=(const Ints & rhs)
{
  if (this == &rhs)
  {
    return *this;
  }

  Ints temp(rhs);

  std::swap(size, temp.size);
  std::swap(data, temp.data);

  return *this;
}

Ints & Ints::operator=(Ints && rhs) noexcept
{
  if (this != &rhs)
  {
    delete [] data;

    capacity = rhs.capacity;
    size = rhs.size;
    data = rhs.data;

    rhs.data = nullptr;
    rhs.size = 0;
    rhs.capacity = 0;

  }

  return *this;
}

Ints Ints::operator+(const Ints & rhs) const
{
  if (getSize() != rhs.getSize())
  {
    throw std::invalid_argument("Arrays have different size");
  }

  Ints result(getSize(), 0);
  for (size_t i = 0; i < getSize(); ++i)
  {
    if (rhs.get(i) > std::numeric_limits< int >::max() - get(i)
        || rhs.get(i) < std::numeric_limits< int >::min() + get(i))
      {
        throw std::invalid_argument("overflow");
      }

    result.set(i, rhs.get(i) + get(i));
  }

  return result;
}

Ints::~Ints()
{
  delete [] data;
}

size_t Ints::getSize() const noexcept
{
  return size;
}

void Ints::set(size_t id, int i)
{
  data[id] = i;
}

int Ints::get(size_t id) noexcept const
{
  return data[id];
}

void Ints::setSafe(size_t id, int i)
{
  if (id > size)
  {
    throw std::logic_error("Invalid value");
  }

  set(id, i);
}

int Ints::getSafe(size_t id) const
{
  if (id > size)
  {
    throw std::logic_error("Invalid value");
  }

  return get(id);
}


Ints Ints::append(int i)
{
  Ints result(*this);

  if (result.capacity < result.size + 1)
  {
    result.resize();
  }

  result.data[size] = i;
  ++result.size;

  return result;
}

struct p_t
{
  int x;
  int y;
};

class Points {
  Ints data_points;
public:
  Points():
    data_points(Ints())
  {}

  Points(const Ints &data):
    data_points(data)
  {}

  ~Points() = default;

  void set(const size_t id, const p_t p) noexcept {
    data_points.set(id*2, p.x);
    data_points.set(id*2+1, p.y);
  }

  p_t get(const size_t id) const noexcept {
    p_t res;
    res.x = data_points.get(id*2);
    res.y = data_points.get(id*2+1);

    return res;
  }

  Points append(p_t p) const {
    size_t const size = data_points.getSize();

    if (size > std::numeric_limits<size_t>::max() - 1) {
      throw std::logic_error("Overflow");
    }

    Ints tmp(size+2);

    for (size_t i = 0; i < size; i++) {
      tmp.set(i, data_points.get(i));
    }

    tmp.set(size, p.x);
    tmp.set(size+1, p.y);

    return tmp;
  }
};

class Mtx {
  size_t m;
  size_t n;
  Ints data_mtx;
  // m - кол-во строк  n - кол-во столбцов

  public:
  Mtx();
  Mtx(size_t m, size_t n);
  Mtx(size_t m, size_t n, const Ints &data);
  ~Mtx() = default;

  Mtx sum(Mtx const& rhs) const;
  Mtx mul(Mtx const& rhs) const;

  Ints getCol(size_t id) const;
  Ints getRow(size_t id) const;

  Mtx addRow(const int pasta) const;

  Mtx transpose() const;
  int det() const;
};

Mtx::Mtx():
m(0), n(0)
{}

Mtx::Mtx(size_t m, size_t n):
  m(m), n(n), data_mtx(n*m, 0)
{}

Mtx::Mtx(size_t m, size_t n, const Ints &data):
  m(m), n(n), data_mtx(data)
{}

Mtx Mtx::sum(Mtx const& rhs) const {
  Mtx result(m, n);
  for (size_t i = 0; i < data_mtx.getSize(); i++) {
    if (rhs.data_mtx.get(i) > std::numeric_limits< int >::max() - data_mtx.get(i)) {
      throw std::logic_error("Overflow");
    }
    result.data_mtx.set(i, rhs.data_mtx.get(i) + data_mtx.get(i));
  }

  return result;
}

Ints Mtx::getCol(const size_t id) const {
  Ints res;
  for (size_t i = 0; i < m; ++i) {
    res = res.append(data_mtx.get(i * n + id));
  }
  return res;
}

Ints Mtx::getRow(const size_t id) const {
  Ints res;
  for (size_t i = 0; i < n; ++i) {
    res = res.append(data_mtx.get(id * n + i));
  }
  return res;
}

Mtx Mtx::mul(const Mtx& rhs) const {
  Mtx res(m, rhs.n);
  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < rhs.n; j++) {
      Ints row = getRow(i);
      Ints col = rhs.getCol(j);

      int sm = 0;
      for (size_t k = 0; k < rhs.m; k++) {
        sm += row.get(k) * col.get(k);
      }

      res.data_mtx.set(i * rhs.n + j, sm);
    }
  }

  return res;
}

Mtx Mtx::addRow(const int pasta) const {
  Mtx res(m+1, n);
  for (size_t i = 0; i < m*n; i++) {
    res.data_mtx.set(i, data_mtx.get(i));
  }
  for (size_t i = 0; i < n; i++) {
    res.data_mtx.set(n*m + i, pasta);
  }
  return res;
}

Mtx Mtx::transpose() const {
  Mtx res(n, m);

  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < n; j++) {
      res.data_mtx.set(j * m + i, data_mtx.get(i * n + j));
    }}

  return res;
}

int Mtx::det() const{
  if (m != n) throw std::logic_error("Can't estimate determinant");
  if (m == 1) return data_mtx.get(0);
  if (m == 2) {
    return data_mtx.get(0) * data_mtx.get(3) - data_mtx.get(1) * data_mtx.get(2);
  }

  int res = 0;

  for (size_t i = 0; i < n; i++) {
    Mtx minor(m-1, n-1);

    size_t p = 0;

    for (size_t row = 1; row < m; row++) {
      for (size_t col = 0; col < n; col++) {
        if (col == i) continue;

        minor.data_mtx.set(p++, data_mtx.get(row * n + col));
      }
    }

    int sign = i % 2 == 0 ? 1 : -1;

    res += sign * data_mtx.get(i) * minor.det();
  }

  return res;
}

class Plot
{
public:
  void set(const int x, const int y);
  int calc(int x) const;
  int solve(Ints & buf, int y) const;
  Points append(p_t p) const;
private:
  Ints data_plot;
};

void Plot::set(const int x, const int y) {
  for (size_t i = 0; i < data_plot.getSize(); i+=2) {
    if (data_plot.get(i) == x) {
      data_plot.set(i+1, y);
      return;
    }
  }

  data_plot = data_plot.append(x);
  data_plot = data_plot.append(y);
}

int Plot::calc(const int x) const {
  for (size_t i = 0; i < data_plot.getSize(); i+=2) {
    if (data_plot.get(i) == x) {
      return data_plot.get(i+1);
    }
  }

  throw std::logic_error("Point not found");
}

int Plot::solve(Ints & buf, const int y) const {
  int cnt = 0;
  for (size_t i = 1; i < data_plot.getSize(); i+=2) {
    if (y == data_plot.get(i)) {
      buf = buf.append(data_plot.get(i-1));
      cnt++;
    }
  }
  return cnt;
}

Points Plot::append(const p_t p) const {
  size_t const size = data_plot.getSize();
  Ints tmp(size + 2);

  for (size_t i = 0; i < size; i++) {
    tmp.set(i, data_plot.get(i));
  }

  tmp.set(size, p.x);
  tmp.set(size+1, p.y);

  return tmp;
}


int main()
{
  return 0;
}