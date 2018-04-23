from django.test import TestCase
from django.core import serializers
from .models import User


# Create your tests here.

class UserTestCase(TestCase):
    def setUp(self):
        User.create_user(username="joesmith1", email="joesmith1@gmail.com", password="joesmith1password")
        User.create_user(username="joesmith2", email="joesmith2@gmail.com", password="joesmith2password")
        User.create_user(username="joesmith3", email="joesmith3@gmail.com", password="joesmith3password")
        User.create_user(username="joesmith4", email="joesmith4@gmail.com", password="joesmith4password")

    def tearDown(self):
        User.delete_user(username="joesmith1", email="joesmith1@gmail.com", password="joesmith1password")
        User.delete_user(username="joesmith2", email="joesmith2@gmail.com", password="joesmith2password")
        User.delete_user(username="joesmith3", email="joesmith3@gmail.com", password="joesmith3password")
        User.delete_user(username="joesmith4", email="joesmith4@gmail.com", password="joesmith4password")
        
    def test_try_create_user_fail(self):
        test = User.try_create_user(username="joesmith1", email="joesmith1@gmail.com", password="joesmith1password")
        self.assertEqual(test[0], 'Email Exists')

    def test_try_create_user(self):
        test = User.try_create_user(username="joesmith5", email="joesmith5@gmail.com", password="joesmith5password")
        self.assertEqual(test[0], 'Please check activation mail in joesmith5@gmail.com')

    def test_user_create_existing_email(self):
        test = User.create_user(username="joesmith1", email="joesmith1@gmail.com", password="joesmith1password")
        self.assertEqual(test[0], 'Email Exists')
        
    def test_create_facebook_user_fail(self):
        test = User.create_facebook_user(username="joesmith1", email="joesmith1@gmail.com")
        self.assertEqual(test[0], 'Email Exists')

    def test_create_facebook_user(self):
        test = User.create_facebook_user(username="joesmith6", email="joesmith6@gmail.com")
        self.assertEqual(test[0].password, "-1")
        test2 = User.login_with_facebook("joesmith6@gmail.com")
        self.assertEqual(test2[0], True)
        self.assertEqual(test2[1][0]['email'], "joesmith6@gmail.com")
        self.assertEqual(test2[1][0]['username'], "joesmith6")
        
    def test_login_with_facebook_fail(self):
        test = User.login_with_facebook("joesmith@gmail.com")
        self.assertEqual(test[0], False)
        self.assertEqual(test[1], 'Email not exists')

    def test_user_get_user_by_email(self):
        test = User.get_user_by_email(email="joesmith1@gmail.com")
        self.assertEqual(test[0].username, 'joesmith1')
        id = test[0].u_id
        test = User.get_user_by_id(id)
        self.assertEqual("joesmith1@gmail.com", test[0].email)

    def test_user_login_by_email(self):
        test = User.login_by_email(email="joesmith11@gmail.com", password="joesmith1password")
        self.assertFalse(test[0])
        test = User.login_by_email(email="joesmith1@gmail.com", password="joesmith11password")
        self.assertEqual(test[1], 'Password incorrect')
        test = User.login_by_email(email="joesmith2@gmail.com", password="joesmith2password")
        self.assertEqual(test[1][0]['username'], 'joesmith2')

    def test_check_vericode_fail(self):
        test = User.check_vericode("joesmith@gmail.com", "BBBBBB")
        self.assertEqual(test[0], False)
        self.assertEqual(test[1], 'Email not exists')
        
    def test_post(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        User.post_baitter(user.username, 'Test content', None)
        post = user.posted.all()[0]
        self.assertEqual(post.content, 'Test content')
    
    def test_post_fail(self):
        success, error = User.post_baitter('non-exist user', 'Test content', None)
        self.assertTrue(not success)
        self.assertEqual(error, 'User not exist')
        
    def test_repost(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        User.post_baitter(user.username, 'Test content', None)
        post = user.posted.all()[0]
        repost, _ = User.repost_baitter(user2.username, post.p_id)
        self.assertEqual(repost.content, post.content)
        
    def test_repost_fail(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        post, _ = User.post_baitter(user.username, 'Test content', None)
        success, error = User.repost_baitter('non-exist user', post.p_id)
        self.assertTrue(not success)
        self.assertEqual(error, 'User not exist')
        
    def test_follow_user(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        success, msg = User.follow_user(user.u_id, user2.u_id)
        self.assertTrue(success)
        self.assertEqual(msg, 'Target successfully followed')
        
    def test_follow_user_fail1(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        success, msg = User.follow_user(user.u_id, user2.u_id)
        self.assertTrue(success)
        self.assertEqual(msg, 'Target successfully followed')
        success, msg = User.follow_user(user.u_id, user2.u_id)
        self.assertTrue(not success)
        self.assertEqual(msg, 'Target already following')
        
    def test_follow_user_fail2(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        success, msg = User.follow_user(user.u_id, -1)
        self.assertTrue(not success)
        self.assertEqual(msg, 'ID not found')
        
    def test_unfollow_user(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        success, msg = User.follow_user(user.u_id, user2.u_id)
        self.assertTrue(success)
        self.assertEqual(msg, 'Target successfully followed')
        success2, msg2 = User.unfollow_user(user.u_id, user2.u_id)
        self.assertTrue(success2)
        self.assertEqual(msg2, 'Target successfully unfollowed')
        
    def test_unfollow_user_fail1(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        success, msg = User.unfollow_user(user.u_id, user2.u_id)
        self.assertTrue(not success)
        self.assertEqual(msg, 'Target not following')
        
    def test_unfollow_user_fail2(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        success, msg = User.unfollow_user(user.u_id, -1)
        self.assertTrue(not success)
        self.assertEqual(msg, 'ID not found')
        
    def test_like(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        post, _ = User.post_baitter(user.username, 'Test content', None)
        success, msg = User.like_post(user2.u_id, post.p_id)
        self.assertTrue(success)
        self.assertEqual(msg, 'Post successfully liked')
        
    def test_like_fail1(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        post, _ = User.post_baitter(user.username, 'Test content', None)
        success, msg = User.like_post(user.u_id, post.p_id)
        self.assertTrue(success)
        self.assertEqual(msg, 'Post successfully liked')
        success2, msg2 = User.like_post(user.u_id, post.p_id)
        self.assertTrue(not success2)
        self.assertEqual(msg2, 'Post already liked')
        
    def test_like_fail2(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        post, _ = User.post_baitter(user.username, 'Test content', None)
        success, msg = User.like_post(-1, post.p_id)
        self.assertTrue(not success)
        self.assertEqual(msg, 'User or post not found')
        
    def test_like_fail3(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        post, _ = User.post_baitter(user.username, 'Test content', None)
        success, msg = User.like_post(user2.u_id, -1)
        self.assertTrue(not success)
        self.assertEqual(msg, 'User or post not found')
        
    def test_unlike(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        post, _ = User.post_baitter(user.username, 'Test content', None)
        success, msg = User.like_post(user2.u_id, post.p_id)
        self.assertTrue(success)
        self.assertEqual(msg, 'Post successfully liked')
        success2, msg2 = User.unlike_post(user2.u_id, post.p_id)
        self.assertTrue(success2)
        self.assertEqual(msg2, 'Post successfully unliked')
        
    def test_unlike_fail1(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        post, _ = User.post_baitter(user.username, 'Test content', None)
        success2, msg2 = User.unlike_post(user.u_id, post.p_id)
        self.assertTrue(not success2)
        self.assertEqual(msg2, 'Post not liked')
        
    def test_unlike_fail2(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        post, _ = User.post_baitter(user.username, 'Test content', None)
        success, msg = User.unlike_post(-1, post.p_id)
        self.assertTrue(not success)
        self.assertEqual(msg, 'User or post not found')
        
    def test_unlike_fail3(self):
        user = User.get_user_by_email(email="joesmith1@gmail.com")[0]
        user2 = User.get_user_by_email(email="joesmith2@gmail.com")[0]
        post, _ = User.post_baitter(user.username, 'Test content', None)
        success, msg = User.unlike_post(user2.u_id, -1)
        self.assertTrue(not success)
        self.assertEqual(msg, 'User or post not found')
            
        




