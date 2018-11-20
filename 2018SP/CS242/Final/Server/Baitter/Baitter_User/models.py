from django.db import models
from django.contrib.auth.models import AbstractUser, UserManager
from django.db.models import Q
from django.contrib import admin
import sys
sys.path.insert(0, '..')
from Baitter_Post.models import Post

class User(AbstractUser):
    u_id = models.AutoField(primary_key=True)
    vericode = models.CharField(max_length=255, null=True)
    following = models.ManyToManyField("User")
    liked = models.ManyToManyField(Post, related_name="Liked")
    posted = models.ManyToManyField(Post, related_name="Posted")
    
    defaultFacebookPassword = "-1"
    manager = UserManager()
    
    class Meta:
        db_table = 'USERS'
        ordering = ['u_id']
        
    @staticmethod
    def try_create_user(username, email, password):
        query = Q(email__exact=email)
        result = User.manager.filter(query)
        if result.count() > 0:
            return 'Email Exists', result.count()
        msg = 'Please check activation mail in ' + email
        return msg, 0
        
    @staticmethod
    def create_user(username, email, password):
        query = Q(email__exact=email)
        result = User.manager.filter(query)
        if result.count() > 0:
            return 'Email Exists', result.count()
        user = User.manager.create(username=username, email=email, password=password)
        return user, 0
        
    @staticmethod
    def create_facebook_user(username, email):
        query = Q(email__exact=email)
        result = User.manager.filter(query)
        if result.count() > 0:
            return 'Email Exists', result.count()
        user = User.manager.create(username=username, email=email, password=User.defaultFacebookPassword)
        return user, 0
        
    @staticmethod
    def login_with_facebook(email):
        query = Q(email__exact=email)
        result = User.manager.filter(query)
        if result.count() == 0:
            return False, 'Email not exists'
        return True, result.values('u_id', 'username', 'email')
        
    @staticmethod
    def get_user_by_id(id):
        query = Q(u_id=id)
        user = User.manager.filter(query)
        return user
        
    @staticmethod
    def get_user_by_email(email):
        query = Q(email__exact=email)
        result = User.manager.filter(query)
        return result
        
    @staticmethod
    def login_by_email(email, password):
        query = Q(email__exact=email)
        result = User.manager.filter(query)
        if password==User.defaultFacebookPassword:
            return False, 'Password incorrect'
        if result.count() == 0:
            return False, 'Email not exists'
        query = Q(password__exact=password)
        result = result.filter(query)
        if result.count() == 0:
            return False, 'Password incorrect'
        return True, result.values('u_id', 'username', 'email')    
    
    @staticmethod
    def delete_user(username, email, password):
        query = Q(u_id__gt=12) & Q(username__exact=username) & Q(email__exact=email) & Q(password__exact=password)
        result = User.manager.filter(query).delete()
        return result
        
    @staticmethod
    def check_vericode(email, vericode):
        query = Q(email__exact=email)
        result = User.manager.filter(query)
        if result.count() == 0:
            return False, 'Email not exists'
        query = Q(vericode__exact=vericode)
        result = result.filter(query)
        if result.count() == 0:
            return False, 'Vericode incorrect'
        return True, email

    @staticmethod
    def post_baitter(user_id, content, img):
        query = Q(u_id__exact=user_id)
        result = User.manager.filter(query)
        if len(result) == 0:
            return False, 'User not exist'
        author = result[0]
        post, n = Post.create_post(content, img)
        author.posted.add(post)
        return post, n
        
    @staticmethod
    def repost_baitter(user_id, post_id):
        query_user = Q(u_id__exact=user_id)
        result_user = User.manager.filter(query_user)
        if len(result_user) == 0:
            return False, 'User not exist'
        author = result_user[0]
        query_post = Q(p_id__exact=post_id)
        result_post = Post.manager.filter(query_post)
        post = result_post[0]
        new_post, n = Post.create_post("REPOST:" + post.content, post.img, post.p_id)
        author.posted.add(new_post)
        return new_post, n
        
    @staticmethod
    def follow_user(from_id, to_id):
        query_from = Q(u_id=from_id)
        result_from = User.manager.filter(query_from)
        query_to = Q(u_id=to_id)
        result_to = User.manager.filter(query_to)
        if result_from.count() == 0 or result_to.count() == 0:
            return False, 'ID not found'
        user_from = User.get_user_by_id(from_id)[0]
        user_to = User.get_user_by_id(to_id)[0]
        if user_to in user_from.following.all():
            return False, 'Target already following'
        else:
            user_from.following.add(user_to)
            return True, 'Target successfully followed'
            
    @staticmethod
    def unfollow_user(from_id, to_id):
        query_from = Q(u_id=from_id)
        result_from = User.manager.filter(query_from)
        query_to = Q(u_id=to_id)
        result_to = User.manager.filter(query_to)
        if result_from.count() == 0 or result_to.count() == 0:
            return False, 'ID not found'
        user_from = User.get_user_by_id(from_id)[0]
        user_to = User.get_user_by_id(to_id)[0]
        if user_to not in user_from.following.all():
            return False, 'Target not following'
        else:
            user_from.following.remove(user_to)
            return True, 'Target successfully unfollowed'
            
    @staticmethod
    def isfollowing(from_id, to_id):
        query_from = Q(u_id=from_id)
        result_from = User.manager.filter(query_from)
        query_to = Q(u_id=to_id)
        result_to = User.manager.filter(query_to)
        if result_from.count() == 0 or result_to.count() == 0:
            return False
        user_from = User.get_user_by_id(from_id)[0]
        user_to = User.get_user_by_id(to_id)[0]
        if user_to in user_from.following.all():
            return True
        else:
            return False
            
    @staticmethod
    def like_post(user_id, post_id):
        query_user = Q(u_id=user_id)
        result_user = User.manager.filter(query_user)
        query_post = Q(p_id=post_id)
        result_post = Post.manager.filter(query_post)
        if result_user.count() == 0 or result_post.count() == 0:
            return False, 'User or post not found'
        user = User.get_user_by_id(user_id)[0]
        post = Post.get_post_by_id(post_id)[0]
        if post in user.liked.all():
            return False, 'Post already liked'
        else:
            user.liked.add(post)
            return True, 'Post successfully liked'
            
    @staticmethod
    def isliked(user_id, post_id):
        query_user = Q(u_id=user_id)
        result_user = User.manager.filter(query_user)
        query_post = Q(p_id=post_id)
        result_post = Post.manager.filter(query_post)
        if result_user.count() == 0 or result_post.count() == 0:
            return False
        user = User.get_user_by_id(user_id)[0]
        post = Post.get_post_by_id(post_id)[0]
        if post in user.liked.all():
            return True
        else:
            return False
            
    @staticmethod
    def unlike_post(user_id, post_id):
        query_user = Q(u_id=user_id)
        result_user = User.manager.filter(query_user)
        query_post = Q(p_id=post_id)
        result_post = Post.manager.filter(query_post)
        if result_user.count() == 0 or result_post.count() == 0:
            return False, 'User or post not found'
        user = User.get_user_by_id(user_id)[0]
        post = Post.get_post_by_id(post_id)[0]
        if post not in user.liked.all():
            return False, 'Post not liked'
        else:
            user.liked.remove(post)
            return True, 'Post successfully unliked'
            
    @staticmethod
    def get_all_posts_by_id(user_id):
        query_user = Q(u_id=user_id)
        result_user = User.manager.filter(query_user)
        user = result_user[0]
        return user.posted.all()
        
    @staticmethod
    def get_all_following_posts(user_id):
        query_user = Q(u_id=user_id)
        result_user = User.manager.filter(query_user)
        user = result_user[0]
        following = user.following.all()
        result = []
        authors = []
        for followed in following:
            result += list(followed.posted.all())
            authors += len(list(followed.posted.all())) * [followed]
        return result, authors
        
    @staticmethod
    def get_all_following_by_id(user_id):
        query_user = Q(u_id=user_id)
        result_user = User.manager.filter(query_user)
        user = result_user[0]
        return user.following.all()
        
    @staticmethod
    def get_all_liked_by_id(user_id):
        query_user = Q(u_id=user_id)
        result_user = User.manager.filter(query_user)
        user = result_user[0]
        return user.liked.all()
        
    @staticmethod
    def search_by_name(username):
        return User.objects.filter(username__contains=username)
        
    @staticmethod
    def get_user_by_username(username):
        query = Q(username=username)
        user = User.manager.filter(query)
        return user[0]
        
admin.site.register(User)