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
        posts = User.get_all_posts_by_id(request.user.u_id)
        return render(request, 'index.html', context={'posts': posts})
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
    pass

@csrf_exempt
def login_by_email(request):
    try:
        if request.method == 'GET':
            params = request.GET
        elif request.method == 'POST':
            params = request.POST
        email = params.get('email')
        password = params.get("password")
        result = User.login_by_email(email, password)
        data = {}
        if result[0]:
            for entry in result[1]:
                data['id'] = int(entry['u_id'])
                data['name'] = entry['u_name']
                data['email'] = entry['email']
        else:
            data['warning'] = result[1]
        response = HttpResponse(json.dumps(data), content_type='application/json')
    except Exception as e:
        print('%s (%s)' % (e.message, type(e)))
    return response
    