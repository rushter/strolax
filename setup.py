#!/usr/bin/env python
# -*- coding: utf-8 -*-

try:
    from setuptools import setup, Extension
except:
    from distutils import setup, Extension

from Cython.Build import cythonize

with open('README.rst') as readme_file:
    readme = readme_file.read()

requirements = [
    "cython",
]

setup(
    name='strolax',
    version='0.0.1',
    long_description=readme + '\n\n',
    author_email='me@rushter.com',
    url='https://github.com/rushter/strolax',
    packages=['strolax', ],
    package_dir={'strolax': 'strolax'},
    include_package_data=True,
    install_requires=requirements,
    license="MIT license",
    zip_safe=False,
    keywords='strolax',
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
    ],
    test_suite='tests',
    ext_modules=cythonize(
        [
            Extension("strolax.clean", ["strolax/clean.pyx", ], include_dirs=['./strolax', ]),
        ],
        compiler_directives={'language_level': "3str"},
    ),

)
