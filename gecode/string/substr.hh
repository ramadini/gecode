#ifndef __GECODE_STRING_SUBSTR_HH__
#define __GECODE_STRING_SUBSTR_HH__

namespace Gecode { namespace String {

	/**
	 * \brief Propagator for one-character substring extraction
	 */
	class CharAt : public MixTernaryPropagator
		<StringView, PC_STRING_DOM,
		 Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
		 StringView, PC_STRING_DOM> {
	protected:
		using MixTernaryPropagator
			<StringView, PC_STRING_DOM,
			 Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
			 StringView, PC_STRING_DOM>::x0;
		using MixTernaryPropagator
			<StringView, PC_STRING_DOM,
			 Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
			 StringView, PC_STRING_DOM>::x1;
		using MixTernaryPropagator
			<StringView, PC_STRING_DOM,
			 Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
			 StringView, PC_STRING_DOM>::x2;

		CharAt(Space& home, CharAt& p);
		CharAt(Home home, StringView source, Gecode::Int::IntView index,
					 StringView result);

	private:
		class AllowedIndices {
		private:
			Gecode::Int::ViewValues<Gecode::Int::IntView> values;
			const DashedString& source;
			const DashedString& result;
			bool allow_empty;
			bool allow_char;

			bool allowed(int index) const;
			void next(void);

		public:
			AllowedIndices(Gecode::Int::IntView index,
										 const DashedString& source,
										 const DashedString& result,
										 bool allow_empty, bool allow_char);

			bool operator ()(void) const;
			void operator ++(void);
			int val(void) const;
		};

		static int assigned_char_at(const DashedString& source, int index);
		static bool may_contain(const DashedString& string, int character);
		bool index_intersects(int lower, int upper) const;
		NSIntSet possible_chars(void) const;

	public:
		virtual Actor* copy(Space& home);
		virtual ExecStatus propagate(Space& home, const ModEventDelta& med);

		static ExecStatus post(Home home, StringView source,
													 Gecode::Int::IntView index, StringView result);
	};

	/**
	 * \brief Propagator for substring extraction from a concrete source
	 */
	class Substring : public MixTernaryPropagator
		<Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
		 Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
		 StringView, PC_STRING_DOM> {
	protected:
		using MixTernaryPropagator
			<Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
			 Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
			 StringView, PC_STRING_DOM>::x0;
		using MixTernaryPropagator
			<Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
			 Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
			 StringView, PC_STRING_DOM>::x1;
		using MixTernaryPropagator
			<Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
			 Gecode::Int::IntView, Gecode::Int::PC_INT_DOM,
			 StringView, PC_STRING_DOM>::x2;

		Substring(Space& home, Substring& p);
		Substring(Home home, StringView source,
							Gecode::Int::IntView from,
							Gecode::Int::IntView to, StringView result);

	private:
		class ConcreteSlice {
		private:
			const string& value;
			int offset;
			int count;

		public:
			ConcreteSlice(const string& value, int offset, int count);

			int length(void) const;
			char at(int index) const;
			char front(void) const;
			char back(void) const;
		};

		class SymbolSlice {
		private:
			const StringVal& value;
			int offset;
			int count;

		public:
			SymbolSlice(const StringVal& value, int offset, int count);

			int length(void) const;
			StringSymbol at(int index) const;
			StringSymbol front(void) const;
			StringSymbol back(void) const;
		};

		class SupportedFrom {
		private:
			Gecode::Int::ViewValues<Gecode::Int::IntView> values;
			const Substring& propagator;
			const string& source;
			const string* expected;

			void next(void);

		public:
			SupportedFrom(Gecode::Int::IntView from,
									 const Substring& propagator,
									 const string& source,
									 const string* expected);

			bool operator ()(void) const;
			void operator ++(void);
			int val(void) const;
		};

		class SupportedTo {
		private:
			Gecode::Int::ViewValues<Gecode::Int::IntView> values;
			const Substring& propagator;
			const string& source;
			const string* expected;

			void next(void);

		public:
			SupportedTo(Gecode::Int::IntView to,
									 const Substring& propagator,
									 const string& source,
									 const string* expected);

			bool operator ()(void) const;
			void operator ++(void);
			int val(void) const;
		};

		class SupportedFromSymbols {
		private:
			Gecode::Int::ViewValues<Gecode::Int::IntView> values;
			const Substring& propagator;
			const StringVal& source;
			const StringVal* expected;

			void next(void);

		public:
			SupportedFromSymbols(Gecode::Int::IntView from,
										const Substring& propagator,
										const StringVal& source,
										const StringVal* expected);

			bool operator ()(void) const;
			void operator ++(void);
			int val(void) const;
		};

		class SupportedToSymbols {
		private:
			Gecode::Int::ViewValues<Gecode::Int::IntView> values;
			const Substring& propagator;
			const StringVal& source;
			const StringVal* expected;

			void next(void);

		public:
			SupportedToSymbols(Gecode::Int::IntView to,
									 const Substring& propagator,
									 const StringVal& source,
									 const StringVal* expected);

			bool operator ()(void) const;
			void operator ++(void);
			int val(void) const;
		};

		// Assigned sources are immutable and need no propagation subscription.
		StringView source;

		static int slice_length(int source_length, int from, int to);
		static string slice(const string& source, int from, int to);
		static StringVal slice(const StringVal& source, int from, int to);
		bool candidate_supported(const string& source, int from, int to,
													 const string* expected) const;
		bool candidate_supported(const StringVal& source, int from, int to,
													 const StringVal* expected) const;
		bool from_supported(const string& source, int from,
												const string* expected) const;
		bool from_supported(const StringVal& source, int from,
												const StringVal* expected) const;
		bool to_supported(const string& source, int to,
											const string* expected) const;
		bool to_supported(const StringVal& source, int to,
											const StringVal* expected) const;
		bool result_is_envelope(const NSIntSet& chars,
														int lower, int upper) const;

	public:
		virtual Actor* copy(Space& home);
		virtual PropCost cost(const Space& home,
													const ModEventDelta& med) const;
		virtual ExecStatus propagate(Space& home, const ModEventDelta& med);

		static ExecStatus post(Home home, StringView source,
													 Gecode::Int::IntView from,
													 Gecode::Int::IntView to, StringView result);
		static bool supports_domains(Gecode::Int::IntView from,
																 Gecode::Int::IntView to);
	};

}}

#include <gecode/string/substr/char-at.hpp>
#include <gecode/string/substr/substring.hpp>

#endif