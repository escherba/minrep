#!/usr/bin/env python3

from distutils.core import setup, Extension

setup(
        ext_modules=[Extension("cyclic_string",
                sources=["cyclic_string.c", "../minrep.c"])
            ],
        name="cyclic_string",
        author="Eugene Scherba, Boston University",
        author_email="escherba@bu.edu",
        url="http://tandem.bu.edu/",
        license="BSD",
        version="0.0.1",
        description="Module for working with cyclic strings",
        platforms = ["Linux", "Windows", "Mac OS X"],
        long_description="cyclic_string is a module for working with cyclic strings. It implements Shiloach (1981) algorithm for fast canonization of circular strings. The resulting canonical form of the string is called a *minimal representation*. REFERENCES: Shiloach, Y. Fast canonization of circular strings. Journal of Algorithms 2, 107-121 (1981). http://dx.doi.org/10.1016/0196-6774(81)90013-4."
    )
