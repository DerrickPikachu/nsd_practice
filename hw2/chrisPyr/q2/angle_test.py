import _vector
import pytest
import math

def test_invalid_input():
    v1 = [0,0]
    v2 = [1,0]
    result = _vector.cal_angle(v1,v2)
    assert result == -666

