#include "ScratchPicker.h"
#include "Navi.h"

#include <type_traits>

//struct foo
//{
//};
//
//struct bar : foo
//{
//	bar(bar&& other) = delete;
//	bar& operator=(bar&& other) = default;
//
//	bar& operator=(bar&& other) noexcept = default;
//	bar(const bar& other) noexcept = default;
//};
//
//struct baz : foo
//{
//	baz(baz&& other) = default;
//	baz& operator=(baz&& other) = delete;
//};
//
//struct quz : foo
//{
//	quz(quz&& other) = delete;
//	baz& operator=(baz&& other) = delete;
//};
//
//struct quux : bar
//{
//	quux(quux&& other) = default;
//	quux& operator=(quux&& other) = delete;
//
//	quux& operator=(quux&& other) noexcept = default;
//	quux(const quux& other) noexcept = default;
//};

	struct Foo
	{
		Foo() = default;
		Foo(Foo&&) = default;
		Foo(const Foo&) noexcept
		{
			std::cout << "Foo copy!" << std::endl;
		}
	};

	struct Bar : Foo
	{
	};

	struct Baz {};

	template<typename T>
	struct Detector
	{
		operator T && () { throw "L"; }
		operator T const&() { throw "R"; }
	};

	template<>
	void RunScratch<ScratchWork::NaviTester>()
	{
		try
		{
			Detector<Baz> bazDetector{};
			decltype(Baz(bazDetector)) hmmm;
			hmmm = Baz{};
		}
		catch (const char* error)
		{
			std::cout << error << std::endl;
		}
	}

	static_assert(std::is_constructible_v<Bar, Detector<Bar>>);
	//static_assert(!std::is_constructible_v<Baz, Detector<Baz>>);



	//template<typename T>
	//T const& implicitConvertToT(T const&);

	template<typename T>
	struct is_detector : std::false_type {};

	template<typename U>
	struct is_detector<Detector<U>> : std::true_type {};

	template<typename T>
	constexpr bool is_detector_v = is_detector<T>::value;

	template<typename T, class = std::enable_if_t<!std::is_lvalue_reference_v<T>>>
	T&& implicitConvertToT(T&&);

	template <typename T>
	struct incomplete;

	template<typename T, class = std::enable_if_t<!std::is_rvalue_reference_v<T>>>
	incomplete<T> const& toIncomplete(T const&);

	template<typename T> //, class = std::enable_if_t<!std::is_lvalue_reference_v<T>>>
	incomplete<T>&& toIncomplete(T&&);

	
	Foo&& justFoo(Foo&&);
	//Foo const& justFoo(const Foo&);


	Bar&& justFoo(Bar&&);
	//Bar const& justFoo(const Bar&);

	Baz&& justFoo(Baz&&);
	//Baz const& justFoo(const Baz&);

	template<typename T, class = void>
	struct is_really_move_constructible : std::false_type {};

	template<typename T>
	struct is_really_move_constructible<T, std::enable_if_t<std::is_rvalue_reference_v<decltype(toIncomplete<T>(Detector<T>()))>>> : std::true_type {};

	template<typename T>
	constexpr bool is_really_move_constructible_v = is_really_move_constructible<T>::value;

	template<typename T>
	constexpr bool Detector_v = std::is_move_constructible_v<T> && is_really_move_constructible_v<T>; //!std::is_const_v<decltype(justFoo(Detector<T>()))>;

	//constexpr bool has_mctor = !is_constructible_v<S, M<S>>;


	//static_assert(!Detector_v<Foo>, "Foo shouldn't be move constructible");
	//static_assert(!Detector_v<Bar>, "Bar shouldn't be move constructible");
	//static_assert( Detector_v<Baz>, "Baz should be move constructible");



	//int main()
	//{
	//	Bar bar {};
	//	Bar barTwo { std::move(bar) };
	//}

//static_assert( std::is_move_constructible_v<baz>, "baz should be move constructible");
//static_assert(!std::is_move_constructible_v<quz>, "quz shouldn't be move constructible");
//static_assert(!std::is_constructible_v<quux, quux&&>, "quux shouldn't be move constructible");
//
//static_assert( std::is_move_assignable_v<foo>, "foo should be move assignable");
//static_assert( std::is_move_assignable_v<bar>, "bar should be move assignable");
//static_assert(!std::is_move_assignable_v<baz>, "baz shouldn't be move assignable");
//static_assert(!std::is_move_assignable_v<quz>, "quz shouldn't be move assignable");
//static_assert(!std::is_move_assignable_v<quux>, "quux shouldn't be move assignable");

//
//template<typename T, bool moveConstruct, bool moveAssign, bool copyConstruct, bool copyAssign, bool valueConstruct, bool defaultConstruct, bool destructable>
//static void ExerciseNavi()
//{
//	using NaviType = Navi<T, moveConstruct, moveAssign, copyConstruct, copyAssign, valueConstruct, defaultConstruct, destructable>;
//	//if constexpr(std::is_destructible_v<NaviType>)
//	//{
//		//static_assert(std::is_move_constructible_v<NaviType>      == moveConstruct,    "Navi doesn't match its moveConstruct");
//		static_assert(std::is_move_assignable_v<NaviType>         == moveAssign,       "Navi doesn't match its moveAssign");
//		static_assert(std::is_copy_constructible_v<NaviType>      == copyConstruct,    "Navi doesn't match its copyConstruct");
//		static_assert(std::is_copy_assignable_v<NaviType>         == copyAssign,       "Navi doesn't match its copyAssign");
//		static_assert(std::is_constructible_v<NaviType, T>        == valueConstruct,   "Navi doesn't match its valueConstruct");
//		static_assert(std::is_default_constructible_v<NaviType>   == defaultConstruct, "Navi doesn't match its defaultConstruct");
//	//}
//	static_assert(std::is_destructible_v<NaviType>            == destructable,     "Navi doesn't match its destructable");
//
//	if constexpr(defaultConstruct && std::is_move_constructible_v<NaviType>)
//	{
//		NaviType navi {};
//		NaviType naviTwo(std::move(navi));
//	}
//}
//
//template<size_t N>
//void RunExercise()
//{
//	if constexpr ((N & 0b00000001) != 0 && (N & 0b01000000) == (N & 0b00100000))
//	{
//		ExerciseNavi<int,
//			(N & 0b01000000) != 0 /*moveConstruct*/,
//			(N & 0b00100000) != 0 /*moveAssign*/,
//			(N & 0b00010000) != 0 /*copyConstruct*/,
//			(N & 0b00001000) != 0 /*copyAssign*/,
//			(N & 0b00000100) != 0 /*valueConstruct*/,
//			(N & 0b00000010) != 0 /*defaultConstruct*/,
//			(N & 0b00000001) != 0 /*destructable*/>();
//	}
//
//	RunExercise<N - 1>();
//}
//
//template<>
//void RunExercise<0>()
//{
//	ExerciseNavi<int, false /*moveConstruct*/, false /*moveAssign*/, false /*copyConstruct*/, false /*copyAssign*/, false /*valueConstruct*/, false /*defaultConstruct*/, false /*destructable*/>();
//}

//template<>
//void RunScratch<ScratchWork::NaviTester>()
//{
//	//RunExercise<0b01111111>();
//	//RunExercise<0b00011111>();
//}
