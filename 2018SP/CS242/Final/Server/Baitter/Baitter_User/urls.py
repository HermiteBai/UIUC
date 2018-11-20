#!/usr/bin/env python

from django.conf.urls import url, include
from . import views

app_name = 'Baitter_User'
urlpatterns = [
    url(r'^register/', views.register, name='register'),
    url(r'^publish/', views.publish, name='publish'),
    url(r'^following/', views.following, name='following'),
    url(r'^follower/', views.follower, name='follower'),
    url(r'^search/', views.search, name='search'),
]