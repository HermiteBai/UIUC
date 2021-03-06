# -*- coding: utf-8 -*-
# Generated by Django 1.11.3 on 2018-04-22 20:45
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.manager


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Post',
            fields=[
                ('p_id', models.AutoField(primary_key=True, serialize=False)),
                ('content', models.TextField(max_length=2000)),
                ('img', models.ImageField(upload_to='upload')),
                ('created', models.DateTimeField(auto_now_add=True)),
                ('repost_from', models.IntegerField()),
            ],
            options={
                'db_table': 'POSTS',
                'ordering': ['p_id'],
            },
            managers=[
                ('manager', django.db.models.manager.Manager()),
            ],
        ),
    ]
