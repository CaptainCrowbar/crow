#include "crow/meta.hpp"
#include "crow/unit-test.hpp"
#include <forward_list>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

using namespace Crow;

namespace {

    // Sort types into void < integers by size < pointers < everything else
    template <typename T1, typename T2>
    struct CompareTypes {
        enum {
            t1_is_void     = std::is_void<T1>::value,
            t1_is_integer  = std::is_integral<T1>::value,
            t1_is_pointer  = std::is_pointer<T1>::value,
            t1_key         = 4 * t1_is_void + 2 * t1_is_integer + t1_is_pointer,
            t2_is_void     = std::is_void<T2>::value,
            t2_is_integer  = std::is_integral<T2>::value,
            t2_is_pointer  = std::is_pointer<T2>::value,
            t2_key         = 4 * t2_is_void + 2 * t2_is_integer + t2_is_pointer,
            value          = t1_key > t2_key || (t1_key == t2_key && sizeof(T1) < sizeof(T2))
        };
    };

    template <typename T1, typename T2> using MakePair = std::pair<T1, T2>;
    template <typename T> using MakePtr = T*;

    template <typename T> struct Pattern;
    template <> struct Pattern<Nil>          { static std::string str() { return "*"; } };
    template <> struct Pattern<int>          { static std::string str() { return "i"; } };
    template <> struct Pattern<void*>        { static std::string str() { return "p"; } };
    template <> struct Pattern<std::string>  { static std::string str() { return "s"; } };
    template <typename T1, typename T2> struct Pattern<std::pair<T1, T2>>
        { static std::string str() { return "(" + Pattern<T1>::str() + Pattern<T2>::str() + ")"; } };

    template <typename T1, typename T2> struct UnifyType;
    template <typename T> struct UnifyType<T, T> { using type = T; };
    template <typename T1, typename T2> using Unify = typename UnifyType<T1, T2>::type;

}

void test_crow_meta_append() {

    using list_i     = Typelist<int>;
    using list_is    = Typelist<int, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;

    {  using type = Append<Nil, int>;               TEST_TYPE(type, list_i);     }
    {  using type = Append<list_i, std::string>;    TEST_TYPE(type, list_is);    }
    {  using type = Append<list_is, void*>;         TEST_TYPE(type, list_isp);   }
    {  using type = Append<list_isp, std::string>;  TEST_TYPE(type, list_isps);  }
    {  using type = Append<Nil, Nil>;               TEST_TYPE(type, Nil);        }
    {  using type = Append<list_isp, Nil>;          TEST_TYPE(type, list_isp);   }

}

void test_crow_meta_concat() {

    using list_i       = Typelist<int>;
    using list_s       = Typelist<std::string>;
    using list_p       = Typelist<void*>;
    using list_is      = Typelist<int, std::string>;
    using list_sp      = Typelist<std::string, void*>;
    using list_ss      = Typelist<std::string, std::string>;
    using list_isp     = Typelist<int, std::string, void*>;
    using list_spi     = Typelist<std::string, void*, int>;
    using list_issp    = Typelist<int, std::string, std::string, void*>;
    using list_spsp    = Typelist<std::string, void*, std::string, void*>;
    using list_isssp   = Typelist<int, std::string, std::string, std::string, void*>;
    using list_issssp  = Typelist<int, std::string, std::string, std::string, std::string, void*>;

    {  using type = Concat<>;                                               TEST_TYPE(type, Nil);          }
    {  using type = Concat<Nil>;                                            TEST_TYPE(type, Nil);          }
    {  using type = Concat<Nil, Nil>;                                       TEST_TYPE(type, Nil);          }
    {  using type = Concat<Nil, list_i>;                                    TEST_TYPE(type, list_i);       }
    {  using type = Concat<Nil, list_is>;                                   TEST_TYPE(type, list_is);      }
    {  using type = Concat<list_i, Nil>;                                    TEST_TYPE(type, list_i);       }
    {  using type = Concat<list_is, Nil>;                                   TEST_TYPE(type, list_is);      }
    {  using type = Concat<list_i, list_s>;                                 TEST_TYPE(type, list_is);      }
    {  using type = Concat<list_i, list_sp>;                                TEST_TYPE(type, list_isp);     }
    {  using type = Concat<list_sp, list_i>;                                TEST_TYPE(type, list_spi);     }
    {  using type = Concat<list_sp, list_sp>;                               TEST_TYPE(type, list_spsp);    }
    {  using type = Concat<list_i, list_s, list_p>;                         TEST_TYPE(type, list_isp);     }
    {  using type = Concat<list_is, list_ss, list_sp>;                      TEST_TYPE(type, list_issssp);  }
    {  using type = Concat<Nil, list_is, Nil, list_ss, Nil, list_sp, Nil>;  TEST_TYPE(type, list_issssp);  }
    {  using type = Concat<int>;                                            TEST_TYPE(type, list_i);       }
    {  using type = Concat<int, Nil>;                                       TEST_TYPE(type, list_i);       }
    {  using type = Concat<Nil, int>;                                       TEST_TYPE(type, list_i);       }
    {  using type = Concat<int, list_s>;                                    TEST_TYPE(type, list_is);      }
    {  using type = Concat<list_i, std::string>;                            TEST_TYPE(type, list_is);      }
    {  using type = Concat<int, list_ss, list_sp>;                          TEST_TYPE(type, list_isssp);   }
    {  using type = Concat<list_is, std::string, list_sp>;                  TEST_TYPE(type, list_isssp);   }
    {  using type = Concat<list_is, list_ss, void*>;                        TEST_TYPE(type, list_isssp);   }
    {  using type = Concat<int, std::string, list_sp>;                      TEST_TYPE(type, list_issp);    }
    {  using type = Concat<int, list_ss, void*>;                            TEST_TYPE(type, list_issp);    }
    {  using type = Concat<list_is, std::string, void*>;                    TEST_TYPE(type, list_issp);    }
    {  using type = Concat<int, std::string, void*>;                        TEST_TYPE(type, list_isp);     }

}

void test_crow_meta_insert() {

    using list_i     = Typelist<int>;
    using list_s     = Typelist<std::string>;
    using list_is    = Typelist<int, std::string>;
    using list_ss    = Typelist<std::string, std::string>;
    using list_ips   = Typelist<int, void*, std::string>;
    using list_iips  = Typelist<int, int, void*, std::string>;
    using list_ipps  = Typelist<int, void*, void*, std::string>;
    using list_ipss  = Typelist<int, void*, std::string, std::string>;

    {  using type = Insert<Nil, int, CompareTypes>;               TEST_TYPE(type, list_i);     }
    {  using type = Insert<list_s, std::string, CompareTypes>;    TEST_TYPE(type, list_ss);    }
    {  using type = Insert<list_s, int, CompareTypes>;            TEST_TYPE(type, list_is);    }
    {  using type = Insert<list_i, std::string, CompareTypes>;    TEST_TYPE(type, list_is);    }
    {  using type = Insert<list_ips, int, CompareTypes>;          TEST_TYPE(type, list_iips);  }
    {  using type = Insert<list_ips, void*, CompareTypes>;        TEST_TYPE(type, list_ipps);  }
    {  using type = Insert<list_ips, std::string, CompareTypes>;  TEST_TYPE(type, list_ipss);  }

}

void test_crow_meta_insert_at() {

    using list_c     = Typelist<char>;
    using list_i     = Typelist<int>;
    using list_ci    = Typelist<char, int>;
    using list_ic    = Typelist<int, char>;
    using list_is    = Typelist<int, std::string>;
    using list_cis   = Typelist<char, int, std::string>;
    using list_ics   = Typelist<int, char, std::string>;
    using list_isc   = Typelist<int, std::string, char>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_cisp  = Typelist<char, int, std::string, void*>;
    using list_icsp  = Typelist<int, char, std::string, void*>;
    using list_iscp  = Typelist<int, std::string, char, void*>;
    using list_ispc  = Typelist<int, std::string, void*, char>;

    {  using type = InsertAt<Nil, char, -1>;       TEST_TYPE(type, list_c);     }
    {  using type = InsertAt<Nil, char, 0>;        TEST_TYPE(type, list_c);     }
    {  using type = InsertAt<Nil, char, 1>;        TEST_TYPE(type, list_c);     }
    {  using type = InsertAt<list_i, char, -1>;    TEST_TYPE(type, list_ci);    }
    {  using type = InsertAt<list_i, char, 0>;     TEST_TYPE(type, list_ci);    }
    {  using type = InsertAt<list_i, char, 1>;     TEST_TYPE(type, list_ic);    }
    {  using type = InsertAt<list_i, char, 2>;     TEST_TYPE(type, list_ic);    }
    {  using type = InsertAt<list_is, char, -1>;   TEST_TYPE(type, list_cis);   }
    {  using type = InsertAt<list_is, char, 0>;    TEST_TYPE(type, list_cis);   }
    {  using type = InsertAt<list_is, char, 1>;    TEST_TYPE(type, list_ics);   }
    {  using type = InsertAt<list_is, char, 2>;    TEST_TYPE(type, list_isc);   }
    {  using type = InsertAt<list_is, char, 3>;    TEST_TYPE(type, list_isc);   }
    {  using type = InsertAt<list_isp, char, -1>;  TEST_TYPE(type, list_cisp);  }
    {  using type = InsertAt<list_isp, char, 0>;   TEST_TYPE(type, list_cisp);  }
    {  using type = InsertAt<list_isp, char, 1>;   TEST_TYPE(type, list_icsp);  }
    {  using type = InsertAt<list_isp, char, 2>;   TEST_TYPE(type, list_iscp);  }
    {  using type = InsertAt<list_isp, char, 3>;   TEST_TYPE(type, list_ispc);  }
    {  using type = InsertAt<list_isp, char, 4>;   TEST_TYPE(type, list_ispc);  }

}

void test_crow_meta_prefix() {

    using list_i    = Typelist<int>;
    using list_s    = Typelist<std::string>;
    using list_is   = Typelist<int, std::string>;
    using list_sp   = Typelist<std::string, void*>;
    using list_isp  = Typelist<int, std::string, void*>;

    {  using type = Prefix<int, Nil>;      TEST_TYPE(type, list_i);    }
    {  using type = Prefix<int, list_s>;   TEST_TYPE(type, list_is);   }
    {  using type = Prefix<int, list_sp>;  TEST_TYPE(type, list_isp);  }

}

void test_crow_meta_repeat() {

    using list_s     = Typelist<std::string>;
    using list_sp    = Typelist<std::string, void*>;
    using list_ss    = Typelist<std::string, std::string>;
    using list_sss   = Typelist<std::string, std::string, std::string>;
    using list_sp_1  = Typelist<list_sp>;
    using list_sp_2  = Typelist<list_sp, list_sp>;
    using list_sp_3  = Typelist<list_sp, list_sp, list_sp>;

    {  using type = Repeat<std::string, 0>;  TEST_TYPE(type, Nil);        }
    {  using type = Repeat<std::string, 1>;  TEST_TYPE(type, list_s);     }
    {  using type = Repeat<std::string, 2>;  TEST_TYPE(type, list_ss);    }
    {  using type = Repeat<std::string, 3>;  TEST_TYPE(type, list_sss);   }
    {  using type = Repeat<list_sp, 0>;      TEST_TYPE(type, Nil);        }
    {  using type = Repeat<list_sp, 1>;      TEST_TYPE(type, list_sp_1);  }
    {  using type = Repeat<list_sp, 2>;      TEST_TYPE(type, list_sp_2);  }
    {  using type = Repeat<list_sp, 3>;      TEST_TYPE(type, list_sp_3);  }

}

void test_crow_meta_repeat_list() {

    using list_s       = Typelist<std::string>;
    using list_sp      = Typelist<std::string, void*>;
    using list_ss      = Typelist<std::string, std::string>;
    using list_sss     = Typelist<std::string, std::string, std::string>;
    using list_spsp    = Typelist<std::string, void*, std::string, void*>;
    using list_spspsp  = Typelist<std::string, void*, std::string, void*, std::string, void*>;

    {  using type = RepeatList<Nil, 0>;          TEST_TYPE(type, Nil);          }
    {  using type = RepeatList<Nil, 1>;          TEST_TYPE(type, Nil);          }
    {  using type = RepeatList<Nil, 2>;          TEST_TYPE(type, Nil);          }
    {  using type = RepeatList<Nil, 3>;          TEST_TYPE(type, Nil);          }
    {  using type = RepeatList<std::string, 0>;  TEST_TYPE(type, Nil);          }
    {  using type = RepeatList<std::string, 1>;  TEST_TYPE(type, list_s);       }
    {  using type = RepeatList<std::string, 2>;  TEST_TYPE(type, list_ss);      }
    {  using type = RepeatList<std::string, 3>;  TEST_TYPE(type, list_sss);     }
    {  using type = RepeatList<list_s, 0>;       TEST_TYPE(type, Nil);          }
    {  using type = RepeatList<list_s, 1>;       TEST_TYPE(type, list_s);       }
    {  using type = RepeatList<list_s, 2>;       TEST_TYPE(type, list_ss);      }
    {  using type = RepeatList<list_s, 3>;       TEST_TYPE(type, list_sss);     }
    {  using type = RepeatList<list_sp, 0>;      TEST_TYPE(type, Nil);          }
    {  using type = RepeatList<list_sp, 1>;      TEST_TYPE(type, list_sp);      }
    {  using type = RepeatList<list_sp, 2>;      TEST_TYPE(type, list_spsp);    }
    {  using type = RepeatList<list_sp, 3>;      TEST_TYPE(type, list_spspsp);  }

}

void test_crow_meta_resize() {

    using list_c      = Typelist<char>;
    using list_i      = Typelist<int>;
    using list_cc     = Typelist<char, char>;
    using list_ic     = Typelist<int, char>;
    using list_is     = Typelist<int, std::string>;
    using list_icc    = Typelist<int, char, char>;
    using list_isc    = Typelist<int, std::string, char>;
    using list_isp    = Typelist<int, std::string, void*>;
    using list_iscc   = Typelist<int, std::string, char, char>;
    using list_ispc   = Typelist<int, std::string, void*, char>;
    using list_ispcc  = Typelist<int, std::string, void*, char, char>;

    {  using type = Resize<Nil, 0, char>;       TEST_TYPE(type, Nil);         }
    {  using type = Resize<Nil, 1, char>;       TEST_TYPE(type, list_c);      }
    {  using type = Resize<Nil, 2, char>;       TEST_TYPE(type, list_cc);     }
    {  using type = Resize<list_i, 0, char>;    TEST_TYPE(type, Nil);         }
    {  using type = Resize<list_i, 1, char>;    TEST_TYPE(type, list_i);      }
    {  using type = Resize<list_i, 2, char>;    TEST_TYPE(type, list_ic);     }
    {  using type = Resize<list_i, 3, char>;    TEST_TYPE(type, list_icc);    }
    {  using type = Resize<list_is, 0, char>;   TEST_TYPE(type, Nil);         }
    {  using type = Resize<list_is, 1, char>;   TEST_TYPE(type, list_i);      }
    {  using type = Resize<list_is, 2, char>;   TEST_TYPE(type, list_is);     }
    {  using type = Resize<list_is, 3, char>;   TEST_TYPE(type, list_isc);    }
    {  using type = Resize<list_is, 4, char>;   TEST_TYPE(type, list_iscc);   }
    {  using type = Resize<list_isp, 0, char>;  TEST_TYPE(type, Nil);         }
    {  using type = Resize<list_isp, 1, char>;  TEST_TYPE(type, list_i);      }
    {  using type = Resize<list_isp, 2, char>;  TEST_TYPE(type, list_is);     }
    {  using type = Resize<list_isp, 3, char>;  TEST_TYPE(type, list_isp);    }
    {  using type = Resize<list_isp, 4, char>;  TEST_TYPE(type, list_ispc);   }
    {  using type = Resize<list_isp, 5, char>;  TEST_TYPE(type, list_ispcc);  }

}

void test_crow_meta_skip() {

    using list_i    = Typelist<int>;
    using list_s    = Typelist<std::string>;
    using list_p    = Typelist<void*>;
    using list_is   = Typelist<int, std::string>;
    using list_sp   = Typelist<std::string, void*>;
    using list_isp  = Typelist<int, std::string, void*>;

    {  using type = Skip<Nil, 0>;       TEST_TYPE(type, Nil);       }
    {  using type = Skip<Nil, 1>;       TEST_TYPE(type, Nil);       }
    {  using type = Skip<Nil, 2>;       TEST_TYPE(type, Nil);       }
    {  using type = Skip<list_i, 0>;    TEST_TYPE(type, list_i);    }
    {  using type = Skip<list_i, 1>;    TEST_TYPE(type, Nil);       }
    {  using type = Skip<list_i, 2>;    TEST_TYPE(type, Nil);       }
    {  using type = Skip<list_is, 0>;   TEST_TYPE(type, list_is);   }
    {  using type = Skip<list_is, 1>;   TEST_TYPE(type, list_s);    }
    {  using type = Skip<list_is, 2>;   TEST_TYPE(type, Nil);       }
    {  using type = Skip<list_is, 3>;   TEST_TYPE(type, Nil);       }
    {  using type = Skip<list_isp, 0>;  TEST_TYPE(type, list_isp);  }
    {  using type = Skip<list_isp, 1>;  TEST_TYPE(type, list_sp);   }
    {  using type = Skip<list_isp, 2>;  TEST_TYPE(type, list_p);    }
    {  using type = Skip<list_isp, 3>;  TEST_TYPE(type, Nil);       }
    {  using type = Skip<list_isp, 4>;  TEST_TYPE(type, Nil);       }

}

void test_crow_meta_sublist() {

    using list_i    = Typelist<int>;
    using list_s    = Typelist<std::string>;
    using list_p    = Typelist<void*>;
    using list_is   = Typelist<int, std::string>;
    using list_sp   = Typelist<std::string, void*>;
    using list_isp  = Typelist<int, std::string, void*>;

    {  using type = Sublist<Nil, 0, 0>;       TEST_TYPE(type, Nil);       }
    {  using type = Sublist<Nil, 0, 1>;       TEST_TYPE(type, Nil);       }
    {  using type = Sublist<Nil, 0, 2>;       TEST_TYPE(type, Nil);       }
    {  using type = Sublist<Nil, 1, 0>;       TEST_TYPE(type, Nil);       }
    {  using type = Sublist<Nil, 1, 1>;       TEST_TYPE(type, Nil);       }
    {  using type = Sublist<Nil, 1, 2>;       TEST_TYPE(type, Nil);       }
    {  using type = Sublist<Nil, 2, 0>;       TEST_TYPE(type, Nil);       }
    {  using type = Sublist<Nil, 2, 1>;       TEST_TYPE(type, Nil);       }
    {  using type = Sublist<Nil, 2, 2>;       TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_i, 0, 0>;    TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_i, 0, 1>;    TEST_TYPE(type, list_i);    }
    {  using type = Sublist<list_i, 0, 2>;    TEST_TYPE(type, list_i);    }
    {  using type = Sublist<list_i, 1, 0>;    TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_i, 1, 1>;    TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_i, 1, 2>;    TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_i, 2, 0>;    TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_i, 2, 1>;    TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_i, 2, 2>;    TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_is, 0, 0>;   TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_is, 0, 1>;   TEST_TYPE(type, list_i);    }
    {  using type = Sublist<list_is, 0, 2>;   TEST_TYPE(type, list_is);   }
    {  using type = Sublist<list_is, 0, 3>;   TEST_TYPE(type, list_is);   }
    {  using type = Sublist<list_is, 1, 0>;   TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_is, 1, 1>;   TEST_TYPE(type, list_s);    }
    {  using type = Sublist<list_is, 1, 2>;   TEST_TYPE(type, list_s);    }
    {  using type = Sublist<list_is, 1, 3>;   TEST_TYPE(type, list_s);    }
    {  using type = Sublist<list_is, 2, 0>;   TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_is, 2, 1>;   TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_is, 2, 2>;   TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_is, 2, 3>;   TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_is, 3, 0>;   TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_is, 3, 1>;   TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_is, 3, 2>;   TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_is, 3, 3>;   TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 0, 0>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 0, 1>;  TEST_TYPE(type, list_i);    }
    {  using type = Sublist<list_isp, 0, 2>;  TEST_TYPE(type, list_is);   }
    {  using type = Sublist<list_isp, 0, 3>;  TEST_TYPE(type, list_isp);  }
    {  using type = Sublist<list_isp, 0, 4>;  TEST_TYPE(type, list_isp);  }
    {  using type = Sublist<list_isp, 1, 0>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 1, 1>;  TEST_TYPE(type, list_s);    }
    {  using type = Sublist<list_isp, 1, 2>;  TEST_TYPE(type, list_sp);   }
    {  using type = Sublist<list_isp, 1, 3>;  TEST_TYPE(type, list_sp);   }
    {  using type = Sublist<list_isp, 1, 4>;  TEST_TYPE(type, list_sp);   }
    {  using type = Sublist<list_isp, 2, 0>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 2, 1>;  TEST_TYPE(type, list_p);    }
    {  using type = Sublist<list_isp, 2, 2>;  TEST_TYPE(type, list_p);    }
    {  using type = Sublist<list_isp, 2, 3>;  TEST_TYPE(type, list_p);    }
    {  using type = Sublist<list_isp, 2, 4>;  TEST_TYPE(type, list_p);    }
    {  using type = Sublist<list_isp, 3, 0>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 3, 1>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 3, 2>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 3, 3>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 3, 4>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 4, 0>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 4, 1>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 4, 2>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 4, 3>;  TEST_TYPE(type, Nil);       }
    {  using type = Sublist<list_isp, 4, 4>;  TEST_TYPE(type, Nil);       }

}

void test_crow_meta_take() {

    using list_i    = Typelist<int>;
    using list_is   = Typelist<int, std::string>;
    using list_isp  = Typelist<int, std::string, void*>;

    {  using type = Take<Nil, 0>;       TEST_TYPE(type, Nil);       }
    {  using type = Take<Nil, 1>;       TEST_TYPE(type, Nil);       }
    {  using type = Take<Nil, 2>;       TEST_TYPE(type, Nil);       }
    {  using type = Take<list_i, 0>;    TEST_TYPE(type, Nil);       }
    {  using type = Take<list_i, 1>;    TEST_TYPE(type, list_i);    }
    {  using type = Take<list_i, 2>;    TEST_TYPE(type, list_i);    }
    {  using type = Take<list_is, 0>;   TEST_TYPE(type, Nil);       }
    {  using type = Take<list_is, 1>;   TEST_TYPE(type, list_i);    }
    {  using type = Take<list_is, 2>;   TEST_TYPE(type, list_is);   }
    {  using type = Take<list_is, 3>;   TEST_TYPE(type, list_is);   }
    {  using type = Take<list_isp, 0>;  TEST_TYPE(type, Nil);       }
    {  using type = Take<list_isp, 1>;  TEST_TYPE(type, list_i);    }
    {  using type = Take<list_isp, 2>;  TEST_TYPE(type, list_is);   }
    {  using type = Take<list_isp, 3>;  TEST_TYPE(type, list_isp);  }
    {  using type = Take<list_isp, 4>;  TEST_TYPE(type, list_isp);  }

}

void test_crow_meta_at_index() {

    using list_i     = Typelist<int>;
    using list_is    = Typelist<int, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;

    {  using type = AtIndex<Nil, -1>;        TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<Nil, 0>;         TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<Nil, 1>;         TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_i, -1>;     TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_i, 0>;      TEST_TYPE(type, int);          }
    {  using type = AtIndex<list_i, 1>;      TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_i, 2>;      TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_is, -1>;    TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_is, 0>;     TEST_TYPE(type, int);          }
    {  using type = AtIndex<list_is, 1>;     TEST_TYPE(type, std::string);  }
    {  using type = AtIndex<list_is, 2>;     TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_is, 3>;     TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_isp, -1>;   TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_isp, 0>;    TEST_TYPE(type, int);          }
    {  using type = AtIndex<list_isp, 1>;    TEST_TYPE(type, std::string);  }
    {  using type = AtIndex<list_isp, 2>;    TEST_TYPE(type, void*);        }
    {  using type = AtIndex<list_isp, 3>;    TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_isp, 4>;    TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_isps, -1>;  TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_isps, 0>;   TEST_TYPE(type, int);          }
    {  using type = AtIndex<list_isps, 1>;   TEST_TYPE(type, std::string);  }
    {  using type = AtIndex<list_isps, 2>;   TEST_TYPE(type, void*);        }
    {  using type = AtIndex<list_isps, 3>;   TEST_TYPE(type, std::string);  }
    {  using type = AtIndex<list_isps, 4>;   TEST_TYPE(type, Nil);          }
    {  using type = AtIndex<list_isps, 5>;   TEST_TYPE(type, Nil);          }

}

void test_crow_meta_head() {

    using list_i    = Typelist<int>;
    using list_is   = Typelist<int, std::string>;
    using list_isp  = Typelist<int, std::string, void*>;

    TEST_TYPE(Head<Nil>,       Nil);
    TEST_TYPE(Head<list_i>,    int);
    TEST_TYPE(Head<list_is>,   int);
    TEST_TYPE(Head<list_isp>,  int);

}

void test_crow_meta_tail() {

    using list_i    = Typelist<int>;
    using list_s    = Typelist<std::string>;
    using list_is   = Typelist<int, std::string>;
    using list_sp   = Typelist<std::string, void*>;
    using list_isp  = Typelist<int, std::string, void*>;

    TEST_TYPE(Tail<Nil>,       Nil);
    TEST_TYPE(Tail<list_i>,    Nil);
    TEST_TYPE(Tail<list_is>,   list_s);
    TEST_TYPE(Tail<list_isp>,  list_sp);

}

void test_crow_meta_most() {

    using list_i    = Typelist<int>;
    using list_is   = Typelist<int, std::string>;
    using list_isp  = Typelist<int, std::string, void*>;

    TEST_TYPE(Most<Nil>,       Nil);
    TEST_TYPE(Most<list_i>,    Nil);
    TEST_TYPE(Most<list_is>,   list_i);
    TEST_TYPE(Most<list_isp>,  list_is);

}

void test_crow_meta_last() {

    using list_i    = Typelist<int>;
    using list_is   = Typelist<int, std::string>;
    using list_isp  = Typelist<int, std::string, void*>;

    TEST_TYPE(Last<Nil>,       Nil);
    TEST_TYPE(Last<list_i>,    int);
    TEST_TYPE(Last<list_is>,   std::string);
    TEST_TYPE(Last<list_isp>,  void*);

}

void test_crow_meta_max_min() {

    using list_i     = Typelist<int>;
    using list_is    = Typelist<int, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;

    {  using type = Max<Nil, CompareTypes>;        TEST_TYPE(type, Nil);          }
    {  using type = Max<list_i, CompareTypes>;     TEST_TYPE(type, int);          }
    {  using type = Max<list_is, CompareTypes>;    TEST_TYPE(type, std::string);  }
    {  using type = Max<list_isp, CompareTypes>;   TEST_TYPE(type, std::string);  }
    {  using type = Max<list_isps, CompareTypes>;  TEST_TYPE(type, std::string);  }

    {  using type = Min<Nil, CompareTypes>;        TEST_TYPE(type, Nil);  }
    {  using type = Min<list_i, CompareTypes>;     TEST_TYPE(type, int);  }
    {  using type = Min<list_is, CompareTypes>;    TEST_TYPE(type, int);  }
    {  using type = Min<list_isp, CompareTypes>;   TEST_TYPE(type, int);  }
    {  using type = Min<list_isps, CompareTypes>;  TEST_TYPE(type, int);  }

}

void test_crow_meta_fold() {

    using list_i     = Typelist<int>;
    using list_is    = Typelist<int, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;

    TEST_EQUAL((Pattern<FoldLeft<Nil, MakePair, Nil>>::str()),        "*");
    TEST_EQUAL((Pattern<FoldLeft<list_i, MakePair, Nil>>::str()),     "(*i)");
    TEST_EQUAL((Pattern<FoldLeft<list_is, MakePair, Nil>>::str()),    "((*i)s)");
    TEST_EQUAL((Pattern<FoldLeft<list_isp, MakePair, Nil>>::str()),   "(((*i)s)p)");
    TEST_EQUAL((Pattern<FoldLeft<list_isps, MakePair, Nil>>::str()),  "((((*i)s)p)s)");

    TEST_EQUAL((Pattern<FoldRight<Nil, MakePair, Nil>>::str()),        "*");
    TEST_EQUAL((Pattern<FoldRight<list_i, MakePair, Nil>>::str()),     "(i*)");
    TEST_EQUAL((Pattern<FoldRight<list_is, MakePair, Nil>>::str()),    "(i(s*))");
    TEST_EQUAL((Pattern<FoldRight<list_isp, MakePair, Nil>>::str()),   "(i(s(p*)))");
    TEST_EQUAL((Pattern<FoldRight<list_isps, MakePair, Nil>>::str()),  "(i(s(p(s*))))");

    TEST_EQUAL((Pattern<Reduce<Nil, MakePair>>::str()),        "*");
    TEST_EQUAL((Pattern<Reduce<list_i, MakePair>>::str()),     "i");
    TEST_EQUAL((Pattern<Reduce<list_is, MakePair>>::str()),    "(is)");
    TEST_EQUAL((Pattern<Reduce<list_isp, MakePair>>::str()),   "((is)p)");
    TEST_EQUAL((Pattern<Reduce<list_isps, MakePair>>::str()),  "(((is)p)s)");

}

void test_crow_meta_make_set() {

    using list_i     = Typelist<int>;
    using list_ips   = Typelist<int, void*, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_iisp  = Typelist<int, int, std::string, void*>;
    using list_issp  = Typelist<int, std::string, std::string, void*>;
    using list_ispp  = Typelist<int, std::string, void*, void*>;

    {  using type = MakeSet<Nil, CompareTypes>;        TEST_TYPE(type, Nil);       }
    {  using type = MakeSet<list_i, CompareTypes>;     TEST_TYPE(type, list_i);    }
    {  using type = MakeSet<list_isp, CompareTypes>;   TEST_TYPE(type, list_ips);  }
    {  using type = MakeSet<list_iisp, CompareTypes>;  TEST_TYPE(type, list_ips);  }
    {  using type = MakeSet<list_issp, CompareTypes>;  TEST_TYPE(type, list_ips);  }
    {  using type = MakeSet<list_ispp, CompareTypes>;  TEST_TYPE(type, list_ips);  }

}

void test_crow_meta_map() {

    using list_i     = Typelist<int>;
    using list_is    = Typelist<int, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;
    using ptrs_i     = Typelist<int*>;
    using ptrs_is    = Typelist<int*, std::string*>;
    using ptrs_isp   = Typelist<int*, std::string*, void**>;
    using ptrs_isps  = Typelist<int*, std::string*, void**, std::string*>;

    {  using type = Map<Nil, MakePtr>;        TEST_TYPE(type, Nil);        }
    {  using type = Map<list_i, MakePtr>;     TEST_TYPE(type, ptrs_i);     }
    {  using type = Map<list_is, MakePtr>;    TEST_TYPE(type, ptrs_is);    }
    {  using type = Map<list_isp, MakePtr>;   TEST_TYPE(type, ptrs_isp);   }
    {  using type = Map<list_isps, MakePtr>;  TEST_TYPE(type, ptrs_isps);  }

}

void test_crow_meta_partial_reduce() {

    using list_i      = Typelist<int>;
    using list_s      = Typelist<std::string>;
    using list_ss     = Typelist<std::string, std::string>;
    using list_isp    = Typelist<int, std::string, void*>;
    using list_iisp   = Typelist<int, int, std::string, void*>;
    using list_ispp   = Typelist<int, std::string, void*, void*>;
    using list_issp   = Typelist<int, std::string, std::string, void*>;
    using list_spsp   = Typelist<std::string, void*, std::string, void*>;
    using list_iiisp  = Typelist<int, int, int, std::string, void*>;
    using list_isppp  = Typelist<int, std::string, void*, void*, void*>;
    using list_isssp  = Typelist<int, std::string, std::string, std::string, void*>;

    {  using type = PartialReduce<Nil, std::is_same, Unify>;         TEST_TYPE(type, Nil);        }
    {  using type = PartialReduce<list_i, std::is_same, Unify>;      TEST_TYPE(type, list_i);     }
    {  using type = PartialReduce<list_ss, std::is_same, Unify>;     TEST_TYPE(type, list_s);     }
    {  using type = PartialReduce<list_isp, std::is_same, Unify>;    TEST_TYPE(type, list_isp);   }
    {  using type = PartialReduce<list_iisp, std::is_same, Unify>;   TEST_TYPE(type, list_isp);   }
    {  using type = PartialReduce<list_iiisp, std::is_same, Unify>;  TEST_TYPE(type, list_isp);   }
    {  using type = PartialReduce<list_issp, std::is_same, Unify>;   TEST_TYPE(type, list_isp);   }
    {  using type = PartialReduce<list_isssp, std::is_same, Unify>;  TEST_TYPE(type, list_isp);   }
    {  using type = PartialReduce<list_ispp, std::is_same, Unify>;   TEST_TYPE(type, list_isp);   }
    {  using type = PartialReduce<list_isppp, std::is_same, Unify>;  TEST_TYPE(type, list_isp);   }
    {  using type = PartialReduce<list_spsp, std::is_same, Unify>;   TEST_TYPE(type, list_spsp);  }

}

void test_crow_meta_remove() {

    using list_i     = Typelist<int>;
    using list_s     = Typelist<std::string>;
    using list_ip    = Typelist<int, void*>;
    using list_is    = Typelist<int, std::string>;
    using list_ss    = Typelist<std::string, std::string>;
    using list_sps   = Typelist<std::string, void*, std::string>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;

    {  using type = Remove<Nil, int>;                TEST_TYPE(type, Nil);        }
    {  using type = Remove<list_i, int>;             TEST_TYPE(type, Nil);        }
    {  using type = Remove<list_s, int>;             TEST_TYPE(type, list_s);     }
    {  using type = Remove<list_is, int>;            TEST_TYPE(type, list_s);     }
    {  using type = Remove<list_ss, int>;            TEST_TYPE(type, list_ss);    }
    {  using type = Remove<list_ss, std::string>;    TEST_TYPE(type, Nil);        }
    {  using type = Remove<list_isps, int>;          TEST_TYPE(type, list_sps);   }
    {  using type = Remove<list_isps, std::string>;  TEST_TYPE(type, list_ip);    }
    {  using type = Remove<list_isps, float>;        TEST_TYPE(type, list_isps);  }

    {  using type = RemoveIf<Nil, std::is_scalar>;        TEST_TYPE(type, Nil);      }
    {  using type = RemoveIf<list_i, std::is_scalar>;     TEST_TYPE(type, Nil);      }
    {  using type = RemoveIf<list_s, std::is_scalar>;     TEST_TYPE(type, list_s);   }
    {  using type = RemoveIf<list_is, std::is_scalar>;    TEST_TYPE(type, list_s);   }
    {  using type = RemoveIf<list_ss, std::is_scalar>;    TEST_TYPE(type, list_ss);  }
    {  using type = RemoveIf<list_isps, std::is_scalar>;  TEST_TYPE(type, list_ss);  }

}

void test_crow_meta_reverse() {

    using list_i     = Typelist<int>;
    using list_is    = Typelist<int, std::string>;
    using list_ps    = Typelist<void*, std::string>;
    using list_si    = Typelist<std::string, int>;
    using list_sp    = Typelist<std::string, void*>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_psi   = Typelist<void*, std::string, int>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;
    using list_spsi  = Typelist<std::string, void*, std::string, int>;

    TEST_TYPE(Reverse<Nil>,        Nil);
    TEST_TYPE(Reverse<list_i>,     list_i);
    TEST_TYPE(Reverse<list_is>,    list_si);
    TEST_TYPE(Reverse<list_sp>,    list_ps);
    TEST_TYPE(Reverse<list_isp>,   list_psi);
    TEST_TYPE(Reverse<list_isps>,  list_spsi);

}

void test_crow_meta_select() {

    using list_i     = Typelist<int>;
    using list_s     = Typelist<std::string>;
    using list_ip    = Typelist<int, void*>;
    using list_is    = Typelist<int, std::string>;
    using list_ss    = Typelist<std::string, std::string>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;

    {  using type = Select<Nil, std::is_scalar>;        TEST_TYPE(type, Nil);      }
    {  using type = Select<list_i, std::is_scalar>;     TEST_TYPE(type, list_i);   }
    {  using type = Select<list_s, std::is_scalar>;     TEST_TYPE(type, Nil);      }
    {  using type = Select<list_is, std::is_scalar>;    TEST_TYPE(type, list_i);   }
    {  using type = Select<list_ss, std::is_scalar>;    TEST_TYPE(type, Nil);      }
    {  using type = Select<list_isps, std::is_scalar>;  TEST_TYPE(type, list_ip);  }

}

void test_crow_meta_sort() {

    using list_i     = Typelist<int>;
    using list_ips   = Typelist<int, void*, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_iips  = Typelist<int, int, void*, std::string>;
    using list_iisp  = Typelist<int, int, std::string, void*>;
    using list_ipps  = Typelist<int, void*, void*, std::string>;
    using list_ipss  = Typelist<int, void*, std::string, std::string>;
    using list_ispp  = Typelist<int, std::string, void*, void*>;
    using list_issp  = Typelist<int, std::string, std::string, void*>;
    using list_pqr   = Typelist<void*, char*, int*>;
    using list_prq   = Typelist<void*, int*, char*>;
    using list_qpr   = Typelist<char*, void*, int*>;
    using list_qrp   = Typelist<char*, int*, void*>;
    using list_rpq   = Typelist<int*, void*, char*>;
    using list_rqp   = Typelist<int*, char*, void*>;

    {  using type = Sort<Nil, CompareTypes>;        TEST_TYPE(type, Nil);        }
    {  using type = Sort<list_i, CompareTypes>;     TEST_TYPE(type, list_i);     }
    {  using type = Sort<list_isp, CompareTypes>;   TEST_TYPE(type, list_ips);   }
    {  using type = Sort<list_iisp, CompareTypes>;  TEST_TYPE(type, list_iips);  }
    {  using type = Sort<list_issp, CompareTypes>;  TEST_TYPE(type, list_ipss);  }
    {  using type = Sort<list_ispp, CompareTypes>;  TEST_TYPE(type, list_ipps);  }
    {  using type = Sort<list_pqr, CompareTypes>;   TEST_TYPE(type, list_pqr);   }
    {  using type = Sort<list_prq, CompareTypes>;   TEST_TYPE(type, list_prq);   }
    {  using type = Sort<list_qpr, CompareTypes>;   TEST_TYPE(type, list_qpr);   }
    {  using type = Sort<list_qrp, CompareTypes>;   TEST_TYPE(type, list_qrp);   }
    {  using type = Sort<list_rpq, CompareTypes>;   TEST_TYPE(type, list_rpq);   }
    {  using type = Sort<list_rqp, CompareTypes>;   TEST_TYPE(type, list_rqp);   }

}

void test_crow_meta_unique() {

    using list_i       = Typelist<int>;
    using list_s       = Typelist<std::string>;
    using list_sp      = Typelist<std::string, void*>;
    using list_ss      = Typelist<std::string, std::string>;
    using list_isp     = Typelist<int, std::string, void*>;
    using list_iisp    = Typelist<int, int, std::string, void*>;
    using list_ispp    = Typelist<int, std::string, void*, void*>;
    using list_issp    = Typelist<int, std::string, std::string, void*>;
    using list_spsp    = Typelist<std::string, void*, std::string, void*>;
    using list_iiisp   = Typelist<int, int, int, std::string, void*>;
    using list_isppp   = Typelist<int, std::string, void*, void*, void*>;
    using list_isssp   = Typelist<int, std::string, std::string, std::string, void*>;
    using list_ispisp  = Typelist<int, std::string, void*, int, std::string, void*>;

    TEST_TYPE(Unique<Nil>,          Nil);
    TEST_TYPE(Unique<list_i>,       list_i);
    TEST_TYPE(Unique<list_ss>,      list_s);
    TEST_TYPE(Unique<list_isp>,     list_isp);
    TEST_TYPE(Unique<list_iisp>,    list_isp);
    TEST_TYPE(Unique<list_iiisp>,   list_isp);
    TEST_TYPE(Unique<list_issp>,    list_isp);
    TEST_TYPE(Unique<list_isssp>,   list_isp);
    TEST_TYPE(Unique<list_ispp>,    list_isp);
    TEST_TYPE(Unique<list_isppp>,   list_isp);
    TEST_TYPE(Unique<list_spsp>,    list_spsp);
    TEST_TYPE(Unique<list_ispisp>,  list_ispisp);

    TEST_TYPE(FullyUnique<Nil>,          Nil);
    TEST_TYPE(FullyUnique<list_i>,       list_i);
    TEST_TYPE(FullyUnique<list_ss>,      list_s);
    TEST_TYPE(FullyUnique<list_isp>,     list_isp);
    TEST_TYPE(FullyUnique<list_iisp>,    list_isp);
    TEST_TYPE(FullyUnique<list_iiisp>,   list_isp);
    TEST_TYPE(FullyUnique<list_issp>,    list_isp);
    TEST_TYPE(FullyUnique<list_isssp>,   list_isp);
    TEST_TYPE(FullyUnique<list_ispp>,    list_isp);
    TEST_TYPE(FullyUnique<list_isppp>,   list_isp);
    TEST_TYPE(FullyUnique<list_spsp>,    list_sp);
    TEST_TYPE(FullyUnique<list_ispisp>,  list_isp);

}

void test_crow_meta_zip() {

    using list_i       = Typelist<int>;
    using list_s       = Typelist<std::string>;
    using list_is      = Typelist<int, std::string>;
    using list_sp      = Typelist<std::string, void*>;
    using list_isp     = Typelist<int, std::string, void*>;
    using list_spi     = Typelist<std::string, void*, int>;
    using zip_i_s      = Typelist<MakePair<int, std::string>>;
    using zip_is_sp    = Typelist<MakePair<int, std::string>, MakePair<std::string, void*>>;
    using zip_isp_spi  = Typelist<MakePair<int, std::string>, MakePair<std::string, void*>, MakePair<void*, int>>;

    {  using type = Zip<Nil, Nil, MakePair>;            TEST_TYPE(type, Nil);          }
    {  using type = Zip<list_i, list_s, MakePair>;      TEST_TYPE(type, zip_i_s);      }
    {  using type = Zip<list_is, list_sp, MakePair>;    TEST_TYPE(type, zip_is_sp);    }
    {  using type = Zip<list_isp, list_spi, MakePair>;  TEST_TYPE(type, zip_isp_spi);  }
    {  using type = Zip<Nil, list_s, MakePair>;         TEST_TYPE(type, Nil);          }
    {  using type = Zip<list_i, Nil, MakePair>;         TEST_TYPE(type, Nil);          }
    {  using type = Zip<Nil, list_spi, MakePair>;       TEST_TYPE(type, Nil);          }
    {  using type = Zip<list_isp, Nil, MakePair>;       TEST_TYPE(type, Nil);          }
    {  using type = Zip<list_is, list_spi, MakePair>;   TEST_TYPE(type, zip_is_sp);    }
    {  using type = Zip<list_isp, list_sp, MakePair>;   TEST_TYPE(type, zip_is_sp);    }

}

void test_crow_meta_inherit() {

    using list_s       = Typelist<std::string>;
    using list_ss      = Typelist<std::string, std::string>;
    using list_sw      = Typelist<std::string, std::wstring>;
    using list_sss     = Typelist<std::string, std::string, std::string>;
    using list_ssw     = Typelist<std::string, std::string, std::wstring>;
    using list_sws     = Typelist<std::string, std::wstring, std::string>;
    using list_wss     = Typelist<std::wstring, std::string, std::string>;
    using list_swswsw  = Typelist<std::string, std::wstring, std::string, std::wstring, std::string, std::wstring>;

    {  using type = InheritTypelist<Nil>;          TEST((! std::is_base_of<std::string, type>::value));  TEST((! std::is_base_of<std::wstring, type>::value));  }
    {  using type = InheritTypelist<list_s>;       TEST((std::is_base_of<std::string, type>::value));    TEST((! std::is_base_of<std::wstring, type>::value));  }
    {  using type = InheritTypelist<list_ss>;      TEST((std::is_base_of<std::string, type>::value));    TEST((! std::is_base_of<std::wstring, type>::value));  }
    {  using type = InheritTypelist<list_sw>;      TEST((std::is_base_of<std::string, type>::value));    TEST((std::is_base_of<std::wstring, type>::value));    }
    {  using type = InheritTypelist<list_sss>;     TEST((std::is_base_of<std::string, type>::value));    TEST((! std::is_base_of<std::wstring, type>::value));  }
    {  using type = InheritTypelist<list_ssw>;     TEST((std::is_base_of<std::string, type>::value));    TEST((std::is_base_of<std::wstring, type>::value));    }
    {  using type = InheritTypelist<list_sws>;     TEST((std::is_base_of<std::string, type>::value));    TEST((std::is_base_of<std::wstring, type>::value));    }
    {  using type = InheritTypelist<list_wss>;     TEST((std::is_base_of<std::string, type>::value));    TEST((std::is_base_of<std::wstring, type>::value));    }
    {  using type = InheritTypelist<list_swswsw>;  TEST((std::is_base_of<std::string, type>::value));    TEST((std::is_base_of<std::wstring, type>::value));    }

}

void test_crow_meta_tuples() {

    using list_i     = Typelist<int>;
    using list_is    = Typelist<int, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using pair_is    = std::pair<int, std::string>;
    using tuple_0    = std::tuple<>;
    using tuple_i    = std::tuple<int>;
    using tuple_is   = std::tuple<int, std::string>;
    using tuple_isp  = std::tuple<int, std::string, void*>;

    { using type = TypelistToTuple<Nil>;        TEST_TYPE(type, tuple_0);    }
    { using type = TypelistToTuple<list_i>;     TEST_TYPE(type, tuple_i);    }
    { using type = TypelistToTuple<list_is>;    TEST_TYPE(type, tuple_is);   }
    { using type = TypelistToTuple<list_isp>;   TEST_TYPE(type, tuple_isp);  }
    { using type = TupleToTypelist<tuple_0>;    TEST_TYPE(type, Nil);        }
    { using type = TupleToTypelist<tuple_i>;    TEST_TYPE(type, list_i);     }
    { using type = TupleToTypelist<tuple_is>;   TEST_TYPE(type, list_is);    }
    { using type = TupleToTypelist<tuple_isp>;  TEST_TYPE(type, list_isp);   }
    { using type = TupleToTypelist<pair_is>;    TEST_TYPE(type, list_is);    }

}

void test_crow_meta_all_of() {

    using list_ints  = Typelist<short, int, long>;
    using list_strs  = Typelist<std::string, std::wstring>;
    using list_both  = Typelist<short, int, std::string, long>;

    TEST((all_of<Nil, std::is_scalar>));
    TEST((all_of<list_ints, std::is_scalar>));
    TEST((! all_of<list_strs, std::is_scalar>));
    TEST((! all_of<list_both, std::is_scalar>));

    TEST((! any_of<Nil, std::is_scalar>));
    TEST((any_of<list_ints, std::is_scalar>));
    TEST((! any_of<list_strs, std::is_scalar>));
    TEST((any_of<list_both, std::is_scalar>));

    TEST((none_of<Nil, std::is_scalar>));
    TEST((! none_of<list_ints, std::is_scalar>));
    TEST((none_of<list_strs, std::is_scalar>));
    TEST((! none_of<list_both, std::is_scalar>));

}

void test_crow_meta_count() {

    using list_i     = Typelist<int>;
    using list_is    = Typelist<int, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;

    TEST_EQUAL((count<Nil, std::string>),        0);
    TEST_EQUAL((count<list_i, std::string>),     0);
    TEST_EQUAL((count<list_is, std::string>),    1);
    TEST_EQUAL((count<list_isp, std::string>),   1);
    TEST_EQUAL((count<list_isps, std::string>),  2);

    TEST_EQUAL((count_if<Nil, std::is_scalar>),        0);
    TEST_EQUAL((count_if<list_i, std::is_scalar>),     1);
    TEST_EQUAL((count_if<list_is, std::is_scalar>),    1);
    TEST_EQUAL((count_if<list_isp, std::is_scalar>),   2);
    TEST_EQUAL((count_if<list_isps, std::is_scalar>),  2);

    TEST_EQUAL((count_if_not<Nil, std::is_scalar>),        0);
    TEST_EQUAL((count_if_not<list_i, std::is_scalar>),     0);
    TEST_EQUAL((count_if_not<list_is, std::is_scalar>),    1);
    TEST_EQUAL((count_if_not<list_isp, std::is_scalar>),   1);
    TEST_EQUAL((count_if_not<list_isps, std::is_scalar>),  2);

}

void test_crow_meta_find() {

    using list_i     = Typelist<int>;
    using list_s     = Typelist<std::string>;
    using list_is    = Typelist<int, std::string>;
    using list_si    = Typelist<std::string, int>;
    using list_sss   = Typelist<std::string, std::string, std::string>;
    using list_iisp  = Typelist<int, int, std::string, void*>;
    using list_issp  = Typelist<int, std::string, std::string, void*>;

    TEST_EQUAL((find<Nil, int>),                -1);
    TEST_EQUAL((find<list_i, int>),             0);
    TEST_EQUAL((find<list_i, std::string>),     -1);
    TEST_EQUAL((find<list_is, int>),            0);
    TEST_EQUAL((find<list_is, std::string>),    1);
    TEST_EQUAL((find<list_is, void*>),          -1);
    TEST_EQUAL((find<list_issp, int>),          0);
    TEST_EQUAL((find<list_issp, std::string>),  1);
    TEST_EQUAL((find<list_issp, void*>),        3);
    TEST_EQUAL((find<list_issp, char>),         -1);

    TEST_EQUAL((find_if<Nil, std::is_scalar>),       -1);
    TEST_EQUAL((find_if<list_i, std::is_scalar>),    0);
    TEST_EQUAL((find_if<list_s, std::is_scalar>),    -1);
    TEST_EQUAL((find_if<list_is, std::is_scalar>),   0);
    TEST_EQUAL((find_if<list_si, std::is_scalar>),   1);
    TEST_EQUAL((find_if<list_sss, std::is_scalar>),  -1);

    TEST_EQUAL((find_if_not<Nil, std::is_scalar>),        -1);
    TEST_EQUAL((find_if_not<list_i, std::is_scalar>),     -1);
    TEST_EQUAL((find_if_not<list_s, std::is_scalar>),     0);
    TEST_EQUAL((find_if_not<list_is, std::is_scalar>),    1);
    TEST_EQUAL((find_if_not<list_si, std::is_scalar>),    0);
    TEST_EQUAL((find_if_not<list_iisp, std::is_scalar>),  2);

}

void test_crow_meta_in_list() {

    using list_i     = Typelist<int>;
    using list_is    = Typelist<int, std::string>;
    using list_issp  = Typelist<int, std::string, std::string, void*>;

    TEST((! in_list<Nil, int>));
    TEST((in_list<list_i, int>));
    TEST((! in_list<list_i, std::string>));
    TEST((in_list<list_is, int>));
    TEST((in_list<list_is, std::string>));
    TEST((! in_list<list_is, void*>));
    TEST((in_list<list_issp, int>));
    TEST((in_list<list_issp, std::string>));
    TEST((in_list<list_issp, void*>));
    TEST((! in_list<list_issp, char>));

}

void test_crow_meta_is_empty() {

    using list_i     = Typelist<int>;
    using list_is    = Typelist<int, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;

    TEST(is_empty<Nil>);
    TEST(! is_empty<list_i>);
    TEST(! is_empty<list_is>);
    TEST(! is_empty<list_isp>);
    TEST(! is_empty<list_isps>);

    TEST(! is_not_empty<Nil>);
    TEST(is_not_empty<list_i>);
    TEST(is_not_empty<list_is>);
    TEST(! is_not_empty<int>);

}

void test_crow_meta_is_unique() {

    using list_i     = Typelist<int>;
    using list_ii    = Typelist<int, int>;
    using list_is    = Typelist<int, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_iis   = Typelist<int, int, std::string>;
    using list_isi   = Typelist<int, std::string, int>;
    using list_sii   = Typelist<std::string, int, int>;
    using list_ipsw  = Typelist<int, void*, std::string, std::wstring>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;
    using list_issp  = Typelist<int, std::string, std::string, void*>;
    using list_sips  = Typelist<std::string, int, void*, std::string>;
    using list_sisp  = Typelist<std::string, int, std::string, void*>;
    using list_ipss  = Typelist<int, void*, std::string, std::string>;
    using list_ssip  = Typelist<std::string, std::string, int, void*>;

    TEST(is_unique<Nil>);
    TEST(is_unique<list_i>);
    TEST(! is_unique<list_ii>);
    TEST(is_unique<list_is>);
    TEST(is_unique<list_isp>);
    TEST(! is_unique<list_iis>);
    TEST(! is_unique<list_isi>);
    TEST(! is_unique<list_sii>);
    TEST(is_unique<list_ipsw>);
    TEST(! is_unique<list_isps>);
    TEST(! is_unique<list_issp>);
    TEST(! is_unique<list_sips>);
    TEST(! is_unique<list_sisp>);
    TEST(! is_unique<list_ipss>);
    TEST(! is_unique<list_ssip>);

}

void test_crow_meta_length_of() {

    using list_i     = Typelist<int>;
    using list_is    = Typelist<int, std::string>;
    using list_isp   = Typelist<int, std::string, void*>;
    using list_isps  = Typelist<int, std::string, void*, std::string>;

    TEST_EQUAL(length_of<Nil>,        0);
    TEST_EQUAL(length_of<list_i>,     1);
    TEST_EQUAL(length_of<list_is>,    2);
    TEST_EQUAL(length_of<list_isp>,   3);
    TEST_EQUAL(length_of<list_isps>,  4);

}
