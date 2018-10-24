# Task09 - Check Concept
[![Build Status](https://travis-ci.com/mkvdv/otus-cpp-2018.svg?branch=task09)](https://travis-ci.com/mkvdv/otus-cpp-2018)


1. Ответьте на каверзный вопрос – не проверяет ли реализованная нами концепция
    `LessThenComparableCustom` ещё и `DefaultConstructible` концепцию? Если да, то как этого
  избежать? -- **Не проверяет**. Несмотря на то, что поля типа T используются в макросе, экземпляр  LessThenComparableCustom<T> 
  [will never be constructed](https://www.boost.org/doc/libs/1_68_0/libs/concept_check/creating_concepts.htm), 
 the compiler never has to check how its data members will be constructed (C++ Standard Section 14.7.1 9).
 
2. RandomAccessIterator - в `src/random_access_iterator.h`
3. Тесты - в `tests/random_access_iterator_tests.cpp`
4. Зачем поиск Кёнига, где вы тут увидели операторы? Достаточно разграничения по неймспейсам.
5. Хотелось бы концепций типа `IsBeginEndable<T>` - проверка на то, что это контейнер, по которому можно
 итерироваться, `IsDefaultConstructible<T>`, и пожалуй `NonCopyable<T>`. 
 
