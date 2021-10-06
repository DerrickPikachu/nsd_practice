#!/usr/bin/env python3

import _angle
import math
import pytest

def test_1():
	theta = _angle.calAngle([1.0, 0], [-1.0, 0])
	ans = math.acos(-1.0)
	assert (ans - 1e-4 < theta and theta < ans + 1e-4)

def test_2():
	theta = _angle.calAngle([1.0, 0], [1.0, 1.0])
	ans = math.acos(math.sqrt(2.0))
	assert (ans - 1e-4 < theta and theta < ans + 1e-4)

def test_3():
	theta = _angle.calAngle([1.0, 0], [-1.0, 1.0])
	ans = math.acos(-math.sqrt(2.0))
	assert (ans - 1e-4 < theta and theta < ans + 1e-4)

def test_4():
	theta = _angle.calAngle([1.0, 0], [2.0, 0])
	ans = 0
	assert (ans - 1e-4 < theta and theta < ans + 1e-4)

def test_5():
	theta = _angle.calAngle([1.0, 1.0], [-1.0, 1.0])
	ans = math.acos(1.0)
	assert (ans - 1e-4 < theta and theta < ans + 1e-4)

def test_6():
	theta = _angle.calAngle([1.0, 0], [6.0, 8.0])
	ans = math.acos(0.75)
	assert (ans - 1e-4 < theta and theta < ans + 1e-4)
