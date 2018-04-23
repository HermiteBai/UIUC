from django.test import TestCase
from .models import Post

# Create your tests here.
class UserTestCase(TestCase):
    test_ids = []
    
    def setUp(self):
        p1, _ = Post.create_post(content="content1", img=None, repost_id=-1)
        p2, _ = Post.create_post(content="content2", img=None, repost_id=-1)
        p3, _ = Post.create_post(content="content3", img=None, repost_id=-1)
        p4, _ = Post.create_post(content="content4", img=None, repost_id=-1)
        self.test_ids.append(p1.p_id)
        self.test_ids.append(p2.p_id)
        self.test_ids.append(p3.p_id)
        self.test_ids.append(p4.p_id)

    def tearDown(self):
        for p_id in self.test_ids:
            Post.delete_post(p_id)
            
    def test_get_post_by_id(self):
        post, _ = Post.create_post(content="content5", img=None, repost_id=-1)
        self.assertEqual(post.p_id, Post.get_post_by_id(post.p_id)[0].p_id)