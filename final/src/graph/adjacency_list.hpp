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

  template <typename DirectedCategoryT,
            typename VertexPropT = NoProp, typename EdgePropT = NoProp>
  struct AdjacencyList
  {
  public: // PropertyGraph
    /// @brief  Vertex property type
    using VertexProp = VertexPropT;
    /// @brief  Edge property type
    using EdgeProp = EdgePropT;

  private:
    /// @brief  Represents an out edge of a vertex
    struct OutEdge
    {
      /// @brief  Default constructor
      OutEdge() = default;
      /// @brief Sets the target vertex and the index of the edge in the edge list
      /// @param tar
      /// @param storedEdgeIdx
      OutEdge(std::size_t tar, std::size_t storedEdgeIdx)
          : tar(tar), storedEdgeIdx(storedEdgeIdx) {}

    public:
      std::size_t tar;           // target vertex
      std::size_t storedEdgeIdx; // index of the edge in the edge list
    };

    /// @brief  Represents an in edge of a vertex
    struct InEdge
    {
      /// @brief  Default constructor
      InEdge() = default;
      /// @brief  Sets the source vertex and the index of the edge in the edge list
      /// @param src
      /// @param storedEdgeIdx
      InEdge(std::size_t src, std::size_t storedEdgeIdx)
          : src(src), storedEdgeIdx(storedEdgeIdx) {}

    public:
      std::size_t src;           // source vertex
      std::size_t storedEdgeIdx; // index of the edge in the edge list
    };

    /// @brief  Represents a list of out edges of a vertex
    using OutEdgeList = std::vector<OutEdge>;
    /// @brief  Represents a list of in edges of a vertex
    using InEdgeList = std::vector<InEdge>;

    /// @brief A directed or undirected vertex
    /// If undirected, we just don't use the out edge list
    struct StoredVertexDirected
    {
      OutEdgeList eOut;
      VertexProp vp;

    public:
      /// @brief  Default constructor
      StoredVertexDirected() : eOut(new OutEdgeList)
      {
        // If the vertex property is not NoProp, we initialize it
        if constexpr (!std::is_same_v<NoProp, VertexProp>)
        {
          vp = VertexPropT();
        }
      }

      /// @brief  Constructor, vertex property is set by the user
      StoredVertexDirected(OutEdgeList eOut, VertexProp &vp) : eOut(eOut), vp(vp) {}
    };

    /// @brief A bidirectional vertex
    struct StoredVertexBidirectional
    {
      OutEdgeList eOut;
      InEdgeList eIn;
      VertexProp vp;

    public:
      /// @brief  Default constructor
      StoredVertexBidirectional() : eOut(new OutEdgeList), eIn(new InEdgeList)
      {
        // If the vertex property is not NoProp, we initialize it
        if constexpr (!std::is_same_v<NoProp, VertexProp>)
        {
          vp = VertexPropT();
        }
      }
      /// @brief  Constructor, vertex property is set by the user
      StoredVertexBidirectional(OutEdgeList eOut, InEdgeList eIn, VertexProp &vp) : eOut(eOut), eIn(eIn), vp(vp) {}
    };

    // If the graph is directed, we use StoredVertexDirected, otherwise we use StoredVertexBidirectional. We do this
    // instead of using tag dispatching.
    using StoredVertex =
        std::conditional_t<std::is_same_v<DirectedCategoryT, tags::Undirected> || std::is_same_v<DirectedCategoryT, tags::Directed>,
                           StoredVertexDirected,
                           StoredVertexBidirectional>;

    /// @brief  Represents an edge
    struct StoredEdge
    {
      std::size_t src, tar;
      EdgeProp ep;

      /// @brief  Default constructor
      StoredEdge() : src(0), tar(0)
      {
        // If the edge property is not NoProp, we initialize it
        if constexpr (!std::is_same_v<NoProp, EdgeProp>)
        {
          ep = new EdgePropT();
        }
      }
      /// @brief  Constructor, edge property is set by the user
      StoredEdge(std::size_t src, std::size_t tar, EdgeProp ep) : src(src), tar(tar), ep(&ep) {}
    };

    /// @brief Represents a list of vertices
    using VList = std::vector<StoredVertex>;
    /// @brief Represents a list of edges
    using EList = std::vector<StoredEdge>;

  public: // Graph
    /// @brief  Directed category
    using DirectedCategory = DirectedCategoryT;
    /// @brief  Vertex descriptor
    using VertexDescriptor = std::size_t;

    /// @brief  Edge descriptor
    /// @details  The edge descriptor is a struct that contains the source and target vertices of the edge, as well as
    /// the index of the edge in the edge list.
    struct EdgeDescriptor
    {
      /// @brief  Default constructor
      EdgeDescriptor() = default;
      /// @brief  Constructor, sets the source and target vertices and the index of the edge in the edge list
      EdgeDescriptor(std::size_t src, std::size_t tar, std::size_t storedEdgeIdx)
          : src(src), tar(tar), storedEdgeIdx(storedEdgeIdx) {}

    public:
      std::size_t src, tar;
      std::size_t storedEdgeIdx;

    public:
      /// @brief  Equality operator
      /// @param a
      /// @param b
      /// @return true if the two edge descriptors' storedEdgeIdx are equal,
      ///         false otherwise
      friend bool operator==(const EdgeDescriptor &a, const EdgeDescriptor &b)
      {
        return a.storedEdgeIdx == b.storedEdgeIdx;
      }

      /// @brief  Inequality operator
      /// @param a
      /// @param b
      /// @return true if the two edge descriptors' storedEdgeIdx are not equal,
      ///         false otherwise
      friend bool operator!=(const EdgeDescriptor &a, const EdgeDescriptor &b)
      {
        return !(a.storedEdgeIdx == b.storedEdgeIdx);
      }
    };

  public: // VertexListGraph
    /// @brief  Represents a range of vertices
    /// @details  The iterator is simply a counter that returns its value when dereferenced
    struct VertexRange
    {
      // the iterator is simply a counter that returns its value when
      // dereferenced
      using iterator = boost::counting_iterator<VertexDescriptor>;

    public:
        /// @brief  Constructor
        VertexRange(std::size_t n) : n(n) {}

        /// @brief Returns the beginning of the range
        /// @return An iterator to the beginning of the range
        iterator begin() const { return iterator(0); }

        /// @brief Returns the end of the range
        /// @return An iterator to the end of the range
        iterator end() const { return iterator(n); }

    private:
      std::size_t n;
    };

  public: // EdgeListGraph
    /// @brief  Represents a range of edges
    struct EdgeRange
    {
      // We want to adapt the edge list,
      // so it dereferences to EdgeDescriptor instead of StoredEdge
      using EListIterator = typename EList::const_iterator;

      /// @brief  Represents an iterator over a range of edges
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
        /// @brief  The type of iterator we adapt
        using Base =
            boost::iterator_adaptor<iterator, EListIterator, EdgeDescriptor,
                                    std::random_access_iterator_tag,
                                    EdgeDescriptor>;

      public:
        /// @brief  Default constructor
        iterator() = default;
        /// @brief  Constructor, sets the iterator we adapt and the first iterator in the range
        /// @param i The iterator we adapt
        /// @param first The first iterator in the range
        iterator(EListIterator i, EListIterator first) : Base(i), first(first) {}

      private:
        // let the Boost machinery use our methods:
        friend class boost::iterator_core_access;

        /// @brief  Dereference operator
        /// @return The edge descriptor corresponding to the current position of the iterator
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
      /// @brief  Constructor, sets the edge list
      EdgeRange(const AdjacencyList &g) : g(&g) {}

      /// @brief  Returns the beginning of the range
      iterator begin() const
      {
        return iterator(g->eList.begin(), g->eList.begin());
      }

      /// @brief  Returns the end of the range
      iterator end() const { return iterator(g->eList.end(), g->eList.begin()); }

    private:
      const AdjacencyList *g;
    };

  public: // IncidenceGraph
    /// @brief  Represents a range of out edges
    struct OutEdgeRange
    {
      using OutEdgeListIterator = typename OutEdgeList::const_iterator;

      /// @brief  Represents an iterator over a range of out edges
      struct iterator : boost::iterator_adaptor<
                            iterator,                        // because we use CRTP
                            OutEdgeListIterator,             // the iterator we adapt
                            EdgeDescriptor,                  // we want to convert the StoredEdge into an EdgeDescriptor
                            std::bidirectional_iterator_tag, // we can use RA as the underlying iterator supports it
                            EdgeDescriptor>
      { // when we dereference we return by value, not by reference
        /// @brief  The type of iterator we adapt
        using Base = boost::iterator_adaptor<iterator,
                                             OutEdgeListIterator,
                                             EdgeDescriptor,
                                             std::bidirectional_iterator_tag,
                                             EdgeDescriptor>;

      public:
        /// @brief  Default constructor
        iterator() = default;
        /// @brief  Constructor, sets the iterator we adapt and the first iterator in the range
        iterator(OutEdgeListIterator i, OutEdgeListIterator first) : Base(i), first(first) {}

      private:
        friend class boost::iterator_core_access;

        /// @brief  Dereference operator
        /// @return The edge descriptor corresponding to the current position of the iterator
        EdgeDescriptor dereference() const
        {
          const OutEdgeListIterator &i = this->base_reference();
          return EdgeDescriptor{v, i->tar, i->storedEdgeIdx};
        }

      private:
        OutEdgeListIterator first;
      };

    public:
      /// @brief  Constructor, sets the vertex and the graph
      /// @param v Vertex
      /// @param g Graph
      OutEdgeRange(VertexDescriptor v, const AdjacencyList &g) : v(v), g(&g) {}

      /// @brief  Returns the beginning of the range
      iterator begin() const
      {
        return iterator(g->vList[v].eOut.begin(), g->vList[v].eOut.begin());
      }

      /// @brief  Returns the end of the range
      iterator end() const
      {
        return iterator(g->vList[v].eOut.end(), g->vList[v].eOut.begin());
      }

    private:
      const AdjacencyList *g;
      VertexDescriptor v;
    };

  public: // BidirectionalGraph
    /// @brief  Represents a range of in edges
    struct InEdgeRange
    {
      using InEdgeListIterator = typename InEdgeList::const_iterator;

      /// @brief  Represents an iterator over a range of in edges
      struct iterator : boost::iterator_adaptor<
                            iterator,                        // because we use CRTP
                            InEdgeListIterator,              // the iterator we adapt
                            EdgeDescriptor,                  // we want to convert the StoredEdge into an EdgeDescriptor
                            std::bidirectional_iterator_tag, // we can use RA as the underlying iterator supports it
                            EdgeDescriptor>
      { // when we dereference we return by value, not by reference
        /// @brief  The type of iterator we adapt
        using Base = boost::iterator_adaptor<iterator,
                                             InEdgeListIterator,
                                             EdgeDescriptor,
                                             std::bidirectional_iterator_tag,
                                             EdgeDescriptor>;

      public:
        /// @brief  Default constructor
        iterator() = default;
        /// @brief  Constructor, sets the iterator we adapt and the first iterator in the range
        iterator(InEdgeListIterator i, InEdgeListIterator first) : Base(i), first(first) {}

      private:
        friend class boost::iterator_core_access;

        /// @brief  Dereference operator
        /// @return The edge descriptor corresponding to the current position of the iterator
        EdgeDescriptor dereference() const
        {
          const InEdgeListIterator &i = this->base_reference();
          return EdgeDescriptor{i->src, i->tar, static_cast<std::size_t>(i - first)};
        }

      private:
        InEdgeListIterator first;
      };

    public:
      /// @brief  Constructor, sets the vertex and the graph
      /// @param v Vertex
      /// @param g Graph
      InEdgeRange(VertexDescriptor v, const AdjacencyList &g) : v(v), g(&g) {}

      /// @brief  Returns the beginning of the range
      iterator begin() const
      {
        return iterator(g->vList[v].eIn.begin(), g->vList[v].eIn.begin());
      }

      /// @brief  Returns the end of the range
      iterator end() const
      {
        return iterator(g->vList[v].eIn.end(), g->vList[v].eIn.begin());
      }

    private:
      const AdjacencyList *g;
      VertexDescriptor v;
    };

  public:
    /// @brief Default constructor
    AdjacencyList() = default;
    /// @brief Constructor, sets the number of vertices
    /// @param n
    AdjacencyList(std::size_t n) : vList(n) {}

  private:
    VList vList;
    EList eList;

  public: // Graph
    /// @brief  Returns the source vertex of an edge
    /// @param e The edge
    /// @param g The graph
    friend VertexDescriptor source(EdgeDescriptor e, const AdjacencyList &g)
    {
      return e.src;
    }

    /// @brief Returns the target vertex of an edge
    /// @param e The edge
    /// @param g The graph
    friend VertexDescriptor target(EdgeDescriptor e, const AdjacencyList &g)
    {
      return e.tar;
    }

  public: // VertexListGraph
    /// @brief Returns the number of vertices in the graph
    /// @param g The graph
    friend std::size_t numVertices(const AdjacencyList &g)
    {
      return g.vList.size();
    }

    /// @brief Returns a range of vertices
    /// @param g The graph
    friend VertexRange vertices(const AdjacencyList &g)
    {
      return VertexRange(numVertices(g));
    }

  public: // EdgeListGraph
    /// @brief Returns the number of edges in the graph
    friend std::size_t numEdges(const AdjacencyList &g) { return g.eList.size(); }

    /// @brief Returns a range of edges
    friend EdgeRange edges(const AdjacencyList &g) { return EdgeRange(g); }

  public: // Other
    /// @brief Returns the index of a vertex
    friend std::size_t getIndex(VertexDescriptor v, const AdjacencyList &g)
    {
      return v;
    }

  public: // IncidenceGraph
    /// @brief Returns a range of out edges
    friend OutEdgeRange outEdges(const VertexDescriptor v,
                                 const AdjacencyList &g)

    {
      // Two edges are incident if they share a common vertex
      return OutEdgeRange(v, g);
    }

    /// @brief Returns the number of out edges of a vertex
    friend std::size_t outDegree(const VertexDescriptor v,
                                 const AdjacencyList &g)
    {
      // If the graph is undirected, we can just return the number of out edges
      if constexpr (std::is_same_v<DirectedCategory, tags::Undirected>) // constexpr, because is_same is compile time
      {
        // Return the number of out-edges of v in g
        return g.vList[v].eOut.size();
      }
      else // otherwise, we need to count the number of edges that have v as a source or target
      {
        // Return the number of incident edges of v in g
        std::size_t count = 0;
        for (auto e : g.eList)
        {
          if (e.src == v || e.tar == v)
          {
            count++;
          }
        }
        return count;
      }
    }

  public: // BidirectionalGraph
    /// @brief Returns a range of in edges
    friend InEdgeRange inEdges(const VertexDescriptor v,
                               const AdjacencyList &g)
    {
      // return a range of the in-edges of v in g
      return InEdgeRange(v, g);
    }

    /// @brief Returns the number of in edges of a vertex
    friend std::size_t inDegree(const VertexDescriptor v, const AdjacencyList &g)
    {
      // If the graph is undirected, we can just return the number of in edges
      if constexpr (std::is_same_v<DirectedCategory, tags::Undirected>)
      {
        // return the number of in-edges of v in g
        return g.vList[v].eIn.size();
      }
      else // otherwise, we need to count the number of edges that have v as a source or target
      {
        std::size_t count = 0;
        for (auto e : g.eList)
        {
          if (e.src == v || e.tar == v)
          {
            count++;
          }
        }

        return count;
      }
    }

  public: // MutableGraph
    /// @brief Adds a vertex to the graph
    /// @param g The graph to which the vertex is added
    /// @return A descriptor for the newly added vertex
    friend VertexDescriptor addVertex(AdjacencyList &g)
    {
      OutEdgeList out;
      InEdgeList in;

      // Add a vertex and return a descriptor representing the newly added vertex
      if constexpr (std::is_same_v<DirectedCategory, tags::Bidirectional>)
      {
        g.vList.emplace_back(out, in);
      }
      else
      {
        g.vList.emplace_back(out);
      }

      return g.vList.size() - 1; // We set a unique id to be equal to the size of the list of
                 // vectors - 1, as it is 0-indexed. Note that this will not work
                 // in case removing a node is supported.
    }
    
    /// @brief Adds an edge to the graph
    /// @param u The source vertex
    /// @param v The target vertex
    /// @param g The graph to which the edge is added
    /// @return A descriptor for the newly added edge
    friend EdgeDescriptor addEdge(VertexDescriptor u, VertexDescriptor v,
                                  AdjacencyList &g)
    {
      // Both u and v are valid vertex descriptors for g
      assert(u <= g.vList.size() && v <= g.vList.size());

      // u and v are different
      assert(u != v);

      // No edge (u, v) exist already in g
      for (const auto &it : g.eList)
      { // use iterator to iterate through each edge
        assert(it.src != u && it.tar != v);
      }

      // Add the edge to eList
      g.eList.emplace_back(u, v);

      EdgeDescriptor edge = EdgeDescriptor(u, v, g.eList.size() - 1);

      g.vList[u].eOut.emplace_back(edge);

      if constexpr (std::is_same_v<DirectedCategory, tags::Undirected>)
      {
        g.vList[v].eOut.emplace_back(edge);
      }
      if constexpr (std::is_same_v<DirectedCategory, tags::Bidirectional>)
      {
        g.vList[v].eIn.emplace_back(edge);
      }

      return edge;
    }

  public: // MutablePropertyGraph
    /// @brief Adds a vertex to the graph
    /// @param vp The vertex property
    /// @param g The graph to which the vertex is added
    /// @return A descriptor for the newly added vertex
    friend VertexDescriptor addVertex(VertexProp vp, AdjacencyList &g)
    {
      OutEdgeList out;
      InEdgeList in;
      // Add a vertex and return a descriptor representing the newly added vertex
      if constexpr (std::is_same_v<tags::Bidirectional, DirectedCategory>)
      {
        g.vList.emplace_back(out, in, vp);
      }
      else
      {
        g.vList.emplace_back(out, vp);
      }
      return g.vList.size() - 1;
    }

    /// @brief Adds an edge to the graph
    /// @param u The source vertex
    /// @param v The target vertex
    /// @param ep The edge property
    /// @param g The graph to which the edge is added
    /// @return A descriptor for the newly added edge
    friend EdgeDescriptor addEdge(VertexDescriptor u, VertexDescriptor v, EdgeProp ep, AdjacencyList &g)
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

      EdgeDescriptor edge = EdgeDescriptor(u, v, g.eList.size() - 1);
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
    }

    const VertexProp &operator[](VertexDescriptor v) const
    {
      return vList[v].vp;
    }

    EdgeProp &operator[](EdgeDescriptor e)
    {
      return eList[e].ep;
    }

    const EdgeProp &operator[](EdgeDescriptor e) const
    {
      return eList[e].ep;
    }
  };
} // namespace graph

#endif // GRAPH_ADJACENCY_LIST_HPP
