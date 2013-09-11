import unittest

class TestTheMeaning(unittest.TestCase):
    def setUp(self):
        self.the_answer = 42

    def testTheAnswer(self):
        self.assertEqual(int(str(self.the_answer), 13), 6 * 9)

