from django.db import models
from django.contrib import admin
from django.db.models import Q
import sys

class Post(models.Model):
    p_id = models.AutoField(primary_key=True)
    content = models.TextField(max_length=2000)
    img = models.ImageField(upload_to='upload', null=True)
    created = models.DateTimeField(auto_now_add=True)
    repost_from = models.IntegerField()
        
    manager = models.Manager()
    
    class Meta:
        db_table = 'POSTS'
        ordering = ['p_id']
        
    @staticmethod
    def create_post(content, img, repost_id=-1):
        post = Post.manager.create(content=content, img=img, repost_from=repost_id)
        return post, 0
        
    @staticmethod
    def delete_post(post_id):
        query = Q(p_id=post_id)
        result = Post.manager.filter(query).delete()
        return result
        
    @staticmethod
    def get_post_by_id(id):
        query = Q(p_id=id)
        post = Post.manager.filter(query)
        return post
    
admin.site.register(Post)