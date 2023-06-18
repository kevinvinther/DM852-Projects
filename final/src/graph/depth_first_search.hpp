#ifndef GRAPH_DEPTH_FIRST_SEARCH_HPP
#define GRAPH_DEPTH_FIRST_SEARCH_HPP

#include "traits.hpp"

#include <vector>

namespace graph
{

	struct DFSNullVisitor
	{
		template <typename G, typename V>
		void initVertex(const V &, const G &) {}

		template <typename G, typename V>
		void startVertex(const V &, const G &) {}

		template <typename G, typename V>
		void discoverVertex(const V &, const G &) {}

		template <typename G, typename V>
		void finishVertex(const V &, const G &) {}

		template <typename G, typename E>
		void examineEdge(const E &, const G &) {}

		template <typename G, typename E>
		void treeEdge(const E &, const G &) {}

		template <typename G, typename E>
		void backEdge(const E &, const G &) {}

		template <typename G, typename E>
		void forwardOrCrossEdge(const E &, const G &) {}

		template <typename G, typename E>
		void finishEdge(const E &, const G &) {}
	};

	namespace detail
	{

		enum struct DFSColour
		{
			White,
			Grey,
			Black
		};

		template <typename Graph, typename Visitor>
		void dfsVisit(const Graph &g, Visitor &visitor, typename Traits<Graph>::VertexDescriptor u,
					  std::vector<DFSColour> &colour)
		{
			visitor.discoverVertex(u, g);
			colour[u] = graph::detail::DFSColour::Grey;
			visitor.startVertex(u, g);
			for (typename Traits<Graph>::EdgeDescriptor e : outEdges(u, g))
			{
				visitor.examineEdge(e, g);
				if (colour[e.tar] == graph::detail::DFSColour::White)
				{
					visitor.treeEdge(e, g);
					dfsVisit(g, visitor, e.tar, colour);
				}
				else if (colour[e.tar] == graph::detail::DFSColour::Grey)
					visitor.backEdge(e, g);
				else
				{
					visitor.forwardOrCrossEdge(e, g);
				}

				visitor.finishEdge(e, g);
			}
			colour[u] = graph::detail::DFSColour::Black;
			visitor.finishVertex(u, g);
		}

	} // namespace detail

	template <typename Graph, typename Visitor>
	void dfs(const Graph &g, Visitor visitor)
	{
		std::vector<graph::detail::DFSColour> colour(numVertices(g));
		for (typename Traits<Graph>::VertexDescriptor u : vertices(g))
		{
			colour[u] = graph::detail::DFSColour::White;
			visitor.initVertex(u, g);
		}
		for (typename Traits<Graph>::VertexDescriptor u : vertices(g))
		{
			if (colour[u] == graph::detail::DFSColour::White)
			{
				visitor.startVertex(u, g);
				dfsVisit(g, visitor, u, colour);
			}
		}
	}

} // namespace graph

#endif // GRAPH_DEPTH_FIRST_SEARCH_HPP
