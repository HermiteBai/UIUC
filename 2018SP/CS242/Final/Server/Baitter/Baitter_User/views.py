from django.shortcuts import render, redirect
from django.http import HttpResponse
from django.core import serializers
from django.core import signing
from django.core.mail import EmailMessage
from django.views.decorators.csrf import csrf_exempt
from django.template import loader
from .forms import RegistrationForm
from .models import User
import json
import random
import string
import io
import os
# Create your views here.
def index(request):
    if request.user.is_authenticated:
        if request.method == 'POST':
            like(request)
            return redirect(request.META['HTTP_REFERER'])
        posts, authors = User.get_all_following_posts(request.user.u_id)
        liked = [User.isliked(request.user.u_id, post.p_id) for post in posts]
        combined = list(zip(posts, liked, authors))
        sorted_combined = list(sorted(combined, key=lambda x : x[0].created))
        return render(request, 'index.html', context={'posts': list(reversed(sorted_combined))})
    return render(request, 'index.html')
    
def register(request):
    redirect_to = request.POST.get('next', request.GET.get('next', ''))
    if request.method == 'POST':
        form = RegistrationForm(request.POST)
        if form.is_valid():
            form.save()
            if redirect_to:
                return redirect(redirect_to)
            else:
                return redirect('/')
    else:
        form = RegistrationForm()

    return render(request, 'Baitter_User/register.html', context={'form': form, 'next': redirect_to})
    
def publish(request):
    if request.method == 'POST':
        params = request.POST
        content = params['content']
        User.post_baitter(request.user.u_id, content, None)
    return redirect('/')
    
def following(request):
    if request.method == 'POST':
        follow(request)
        return redirect(request.META['HTTP_REFERER'])
    if request.method == 'GET':
        params = request.GET
        users = User.get_all_following_by_id(request.user.u_id)
        followed = [User.isfollowing(request.user.u_id, user.u_id) for user in users]
        return render(request, 'Baitter_User/following.html', context={'following': list(reversed(list(zip(users, followed))))})
    return redirect('/')
    
def follower(request):
    return redirect('/')
    
def search(request):
    if request.method == 'POST':
        follow(request)
        return redirect(request.META['HTTP_REFERER'])
    if request.method == 'GET':
        params = request.GET
        if 'username' in params:
            users = User.search_by_name(params['username'])
            followed = [User.isfollowing(request.user.u_id, user.u_id) for user in users]
            return render(request, 'Baitter_User/search.html', context={'users':zip(users, followed)})
    return render(request, 'Baitter_User/search.html')
    
def follow(request):
    params = request.POST
    to_id = User.get_user_by_username(params['username']).u_id
    if params['button'] == 'Follow':
        User.follow_user(request.user.u_id, to_id)
    else:
        User.unfollow_user(request.user.u_id, to_id)
    
def like(request):
    params = request.POST
    post_id = params['id']
    if 'button' in params:
        if params['button'] == 'Like':
            User.like_post(request.user.u_id, post_id)
        elif params['button'] == 'Unlike':
            User.unlike_post(request.user.u_id, post_id)
    if 'button2' in params:
        User.repost_baitter(request.user.u_id, post_id)