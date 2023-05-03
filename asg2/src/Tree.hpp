#include <functional>

namespace DM852 {
template <typename Key, typename Value, typename Comp = std::less<Key>>
class Tree {
public:
  Tree(const Tree &) = delete;
  Tree(Tree &&) = delete;
  Tree &operator=(const Tree &) = delete;
  Tree &operator=(Tree &&) = delete;

  class iterator {
  public:
  private:
  };

  class const_iterator {
  public:
  private:
  };

  int size() const;   // must run constant time
  bool empty() const; // must run constant time

  std::pair<iterator, bool> insert(const Key &key, const Value &value);
  std::pair<iterator, bool> insert(Key &&key, Value &&value);

  iterator find(const Key &key);
  const_iterator find(const Key &key) const;

  void clear();

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

private:
};
} // namespace DM852
