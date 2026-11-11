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
