#ifndef GRAPH_ADJACENCY_LIST_HPP
#define GRAPH_ADJACENCY_LIST_HPP

#include "properties.hpp"
#include "tags.hpp"
#include "traits.hpp"

#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

#include <cassert>
#include <list>
#include <vector>

namespace graph
{

  template <typename DirectedCategoryT, // Task Type (Task 1a-d)
            typename VertexPropT = NoProp, typename EdgePropT = NoProp>
  struct AdjacencyList
  {
  public: // PropertyGraph
    using VertexProp = VertexPropT;
    using EdgeProp = EdgePropT;
  private:
    struct OutEdge
    {
      OutEdge() = default;
      OutEdge(std::size_t tar, std::size_t storedEdgeIdx)
          : tar(tar), storedEdgeIdx(storedEdgeIdx) {}

    public:
      std::size_t tar;           // target vertex
      std::size_t storedEdgeIdx; // index of the edge in the edge list
    };

    struct InEdge
    {
      InEdge() = default;
      InEdge(std::size_t src, std::size_t storedEdgeIdx)
          : src(src), storedEdgeIdx(storedEdgeIdx) {}

    public:
      std::size_t src;           // source vertex
      std::size_t storedEdgeIdx; // index of the edge in the edge list
    };

    using OutEdgeList = std::vector<OutEdge>;
    using InEdgeList = std::vector<InEdge>;

    struct StoredVertexDirected
    {
      OutEdgeList eOut;
      VertexProp *vp;

    public:
      StoredVertexDirected() : eOut(new OutEdgeList)
      {
        if constexpr (std::is_same_v<NoProp, VertexProp>)
        {
          vp = nullptr;
        }
        else
        {
          vp = new VertexPropT();
        }
      }
      StoredVertexDirected(OutEdgeList eOut, VertexProp &prop) : eOut(eOut), vp(&vp) {}
    };

    struct StoredVertexBidirectional
    {
      OutEdgeList eOut;
      InEdgeList eIn;
      VertexProp *vp;

    public:
      StoredVertexBidirectional() : eOut(new OutEdgeList), eIn(new InEdgeList)
      {
        if constexpr (std::is_same_v<NoProp, VertexProp>)
        {
          vp = nullptr;
        }
        else
        {
          vp = new VertexPropT();
        }
      }
      StoredVertexBidirectional(OutEdgeList eOut, InEdgeList eIn, VertexProp &vp) : eOut(eOut), eIn(eIn), vp(&vp) {}
    };

    using StoredVertex =
        std::conditional_t<std::is_same_v<DirectedCategoryT, tags::Undirected> || std::is_same_v<DirectedCategoryT, tags::Directed>,
                           StoredVertexDirected,
                           StoredVertexBidirectional>;

    struct StoredEdge
    {
      std::size_t src, tar;
      EdgeProp *ep;

      StoredEdge() : src(0), tar(0)
      {
        if constexpr (std::is_same_v<NoProp, EdgeProp>)
        {
          ep = nullptr;
        }
        else
        {
          ep = new EdgePropT();
        }
      }
      StoredEdge(std::size_t src, std::size_t tar, EdgeProp ep) : src(src), tar(tar), ep(&ep) {}
    };

    using VList = std::vector<StoredVertex>;
    using EList = std::vector<StoredEdge>;

  public: // Graph
    using DirectedCategory = DirectedCategoryT;
    using VertexDescriptor = std::size_t;

    struct EdgeDescriptor
    {
      EdgeDescriptor() = default;
      EdgeDescriptor(std::size_t src, std::size_t tar, std::size_t storedEdgeIdx)
          : src(src), tar(tar), storedEdgeIdx(storedEdgeIdx) {}

    public:
      std::size_t src, tar;
      std::size_t storedEdgeIdx;

    public:
      friend bool operator==(const EdgeDescriptor &a, const EdgeDescriptor &b)
      {
        return a.storedEdgeIdx == b.storedEdgeIdx;
      }
    };


  public: // VertexListGraph
    struct VertexRange
    {
      // the iterator is simply a counter that returns its value when
      // dereferenced
      using iterator = boost::counting_iterator<VertexDescriptor>;

    public:
      VertexRange(std::size_t n) : n(n) {}
      iterator begin() const { return iterator(0); }
      iterator end() const { return iterator(n); }

    private:
      std::size_t n;
    };

  public: // EdgeListGraph
    struct EdgeRange
    {
      // We want to adapt the edge list,
      // so it dereferences to EdgeDescriptor instead of StoredEdge
      using EListIterator = typename EList::const_iterator;

      struct iterator
          : boost::iterator_adaptor<
                iterator,      // because we use CRTP
                EListIterator, // the iterator we adapt
                // we want to convert the StoredEdge into an EdgeDescriptor:
                EdgeDescriptor,
                // we can use RA as the underlying iterator supports it:
                std::random_access_iterator_tag,
                // when we dereference we return by value, not by reference
                EdgeDescriptor>
      {
        using Base =
            boost::iterator_adaptor<iterator, EListIterator, EdgeDescriptor,
                                    std::random_access_iterator_tag,
                                    EdgeDescriptor>;

      public:
        iterator() = default;
        iterator(EListIterator i, EListIterator first) : Base(i), first(first) {}

      private:
        // let the Boost machinery use our methods:
        friend class boost::iterator_core_access;

        EdgeDescriptor dereference() const
        {
          // get our current position stored in the
          // boost::iterator_adaptor base class
          const EListIterator &i = this->base_reference();
          return EdgeDescriptor{i->src, i->tar,
                                static_cast<std::size_t>(i - first)};
        }

      private:
        EListIterator first;
      };

    public:
      EdgeRange(const AdjacencyList &g) : g(&g) {}

      iterator begin() const
      {
        return iterator(g->eList.begin(), g->eList.begin());
      }

      iterator end() const { return iterator(g->eList.end(), g->eList.begin()); }

    private:
      const AdjacencyList *g;
    };

  public: // IncidenceGraph
    struct OutEdgeRange
    {
      using OutEdgeListIterator = typename OutEdgeList::const_iterator;

      struct iterator : boost::iterator_adaptor<
                            iterator,                        // because we use CRTP
                            OutEdgeListIterator,             // the iterator we adapt
                            EdgeDescriptor,                  // we want to convert the StoredEdge into an EdgeDescriptor
                            std::bidirectional_iterator_tag, // we can use RA as the underlying iterator supports it
                            EdgeDescriptor>
      { // when we dereference we return by value, not by reference
        using Base = boost::iterator_adaptor<iterator,
                                             OutEdgeListIterator,
                                             EdgeDescriptor,
                                             std::bidirectional_iterator_tag,
                                             EdgeDescriptor>;

      public:
        iterator() = default;
        iterator(OutEdgeListIterator i, OutEdgeListIterator first) : Base(i), first(first) {}

      private:
        friend class boost::iterator_core_access;

        EdgeDescriptor dereference() const
        {
          const OutEdgeListIterator &i = this->base_reference();
          return EdgeDescriptor{i->src, i->tar, static_cast<std::size_t>(i - first)};
        }

      private:
        OutEdgeListIterator first;
      };

    public:
      OutEdgeRange(VertexDescriptor v, const AdjacencyList &g) : v(v), g(&g) {}

      iterator begin() const
      {
        return iterator(g->vList[v].eOut.begin(), g->vList[v].eOut.begin());
      }

      iterator end() const
      {
        return iterator(g->vList[v].eOut.end(), g->vList[v].eOut.begin());
      }

    private:
      const AdjacencyList *g;
      VertexDescriptor v;
    };

  public: // BidirectionalGraph
    struct InEdgeRange
    {
      using InEdgeListIterator = typename InEdgeList::const_iterator;

      struct iterator : boost::iterator_adaptor<
                            iterator,                        // because we use CRTP
                            InEdgeListIterator,              // the iterator we adapt
                            EdgeDescriptor,                  // we want to convert the StoredEdge into an EdgeDescriptor
                            std::bidirectional_iterator_tag, // we can use RA as the underlying iterator supports it
                            EdgeDescriptor>
      { // when we dereference we return by value, not by reference
        using Base = boost::iterator_adaptor<iterator,
                                             InEdgeListIterator,
                                             EdgeDescriptor,
                                             std::bidirectional_iterator_tag,
                                             EdgeDescriptor>;

      public:
        iterator() = default;
        iterator(InEdgeListIterator i, InEdgeListIterator first) : Base(i), first(first) {}

      private:
        friend class boost::iterator_core_access;

        EdgeDescriptor dereference() const
        {
          const InEdgeListIterator &i = this->base_reference();
          return EdgeDescriptor{i->src, i->tar, static_cast<std::size_t>(i - first)};
        }

      private:
        InEdgeListIterator first;
      };

    public:
      InEdgeRange(VertexDescriptor v, const AdjacencyList &g) : v(v), g(&g) {}

      iterator begin() const
      {
        return iterator(g->vList[v].eIn.begin(), g->vList[v].eIn.begin());
      }

      iterator end() const
      {
        return iterator(g->vList[v].eIn.end(), g->vList[v].eIn.begin());
      }

    private:
      const AdjacencyList *g;
      VertexDescriptor v;
    };

  public:
    AdjacencyList() = default;
    AdjacencyList(std::size_t n) : vList(n) {}

  private:
    VList vList;
    EList eList;

  public: // Graph
    friend VertexDescriptor source(EdgeDescriptor e, const AdjacencyList &g)
    {
      return e.src;
    }

    friend VertexDescriptor target(EdgeDescriptor e, const AdjacencyList &g)
    {
      return e.tar;
    }

  public: // VertexListGraph
    friend std::size_t numVertices(const AdjacencyList &g)
    {
      return g.vList.size();
    }

    friend VertexRange vertices(const AdjacencyList &g)
    {
      return VertexRange(numVertices(g));
    }

  public: // EdgeListGraph
    friend std::size_t numEdges(const AdjacencyList &g) { return g.eList.size(); }

    friend EdgeRange edges(const AdjacencyList &g) { return EdgeRange(g); }

  public: // Other
    friend std::size_t getIndex(VertexDescriptor v, const AdjacencyList &g)
    {
      return v;
    }

  public: // IncidenceGraph
    friend OutEdgeRange outEdges(const VertexDescriptor v,
                                 const AdjacencyList &g)

    {
      if constexpr (std::is_same_v<DirectedCategory, tags::Undirected>) // constexpr, because is_same is compile time
      {
        // Return a range of the out-edges of v in g
        return OutEdgeRange(v, g);
      }
      else
      {
        // Return a range of the incident edges of v in g
        for (auto e : g.eList)
        {
          if (e.src == v || e.tar == v)
          {
            // add to inciident list
          }
        }

        // Two edges are incident if they share a common vertex
        return OutEdgeRange(v, g);
      }
    }

    friend std::size_t outDegree(const VertexDescriptor v,
                                 const AdjacencyList &g)
    {
      if constexpr (std::is_same_v<DirectedCategory, tags::Undirected>) // constexpr, because is_same is compile time
      {
        // Return the number of out-edges of v in g
        return g.outEdgeList[v].size();
      }
      else
      {
        // Return the number of incident edges of v in g
      }
    }

  public: // BidirectionalGraph
    friend InEdgeRange inEdges(const VertexDescriptor v,
                               const AdjacencyList &g)
    {
      if constexpr (std::is_same_v<DirectedCategory, tags::Undirected>)
      {
        // return a range of the in-edges of v in g
        return InEdgeRange(v, g);
      }
      else
      {
        // return a range of the incident edges of v in g
        for (auto e : g.eList)
        {
          if (e.src == v || e.tar == v)
          {
            // add to incident list
          }
        }
        // two edges are incident if they share a common vertex
        return InEdgeRange(v, g);
      }
    }

    friend std::size_t inDegree(const VertexDescriptor v, const AdjacencyList &g)
    {
      if constexpr (std::is_same_v<DirectedCategory, tags::Undirected>)
      {
        // return the number of in-edges of v in g
        return g.inEdgeList[v].size();
      }
      else
      {
        // return the number of incident edges of v in g
      }
    }

  public: // MutableGraph
    friend VertexDescriptor addVertex(AdjacencyList g)
    {
      // Add a vertex and return a descriptor representing the newly added vertex
      g.vList.emplace_back();
      return g.vList.size() -
             1; // We set a unique id to be equal to the size of the list of
                // vectors - 1, as it is 0-indexed. Note that this will not work
                // in case removing a node is supported.
    }
    friend EdgeDescriptor addEdge(VertexDescriptor u, VertexDescriptor v,
                                  AdjacencyList g)
    {
      // Both u and v are valid vertex descriptors for g
      assert(u <= g.vList.size() && v <= g.vList.size());

      // u and v are different
      assert(u != v);

      // No edge (u, v) exist already in g
      for (auto it = g.eList.begin(); it != g.eList.end;
           ++it)
      { // use iterator to iterate through each edge
        assert(it->src != u && it->tar != v);
      }

      g.eList.emplace_back(u, v);

      EdgeDescriptor edge = EdgeDescriptor(u, v, g.eList.size() - 1);

      g.vList[u].eOut.emplace_back(edge);

      if constexpr (std::is_same_v<DirectedCategory, tags::Directed>)
      {
        g.vList[v].eIn.emplace_back(edge);
      }
      if constexpr (std::is_same_v<DirectedCategory, tags::Undirected>)
      {
        g.vList[v].eOut.emplace_back(edge);
      }

      return edge;
    }

  public: // MutablePropertyGraph
    friend VertexDescriptor addVertex(VertexProp vp, AdjacencyList g)
    {
      // Add a vertex and return a descriptor representing the newly added vertex
      g.vList.emplace_back(vp);
      return g.vList.size() - 1;
    }

    friend EdgeDescriptor addEdge(VertexDescriptor u, VertexDescriptor v, EdgeProp ep, AdjacencyList g)
    {
      // Both u and v are valid vertex descriptors for g
      assert(u <= g.vList.size() && v <= g.vList.size());

      // u and v are different
      assert(u != v);

      // No edge (u, v) exist already in g
      for (auto it = g.eList.begin(); it != g.eList.end;
           ++it)
      { // use iterator to iterate through each edge
        assert(it->src != u && it->tar != v);
      }

      // Put edge into list of out-edges of u
      g.eList.emplace_back(u, v, ep);

      EdgeDescriptor edge = EdgeDescriptor(u, v, g.eList.Size() - 1);
      g.vList[u].eOut.emplace_back(edge);

      if constexpr (std::is_same_v<DirectedCategory, tags::Bidirectional>)
      {
        g.vList[v].eIn.emplace_back(edge);
      }
      if constexpr (std::is_same_v<DirectedCategory, tags::Undirected>)
      {
        g.vList[v].eOut.emplace_back(edge);
      }

      return edge;
    }

  public: // PropertyGraph
  VertexProp &operator[](VertexDescriptor v)
  {
    return vList[v].vp;
  };

  VertexProp operator[](VertexDescriptor v) const
  {
    return vList[v].vp;
  };

EdgeProp &operator[](EdgeDescriptor e)
{
  return eList[e].ep;
};

EdgeProp operator[](EdgeDescriptor e) const
{
  return eList[e].ep;
};



} // namespace graph

#endif // GRAPH_ADJACENCY_LIST_HPP
