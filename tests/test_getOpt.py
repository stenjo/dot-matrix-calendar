from getopt import GetoptError


def test_getOpt():
    e = GetoptError("test")
    
    assert e.msg == "test"