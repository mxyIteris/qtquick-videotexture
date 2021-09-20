#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os.path

from PyQt5.QtCore import QUrl, QObject, QTimer, pyqtSlot, pyqtSignal
from PyQt5.QtGui import QGuiApplication
from PyQt5.QtQuick import QQuickView

import gobject
gobject.threads_init()

import pygst
pygst.require('0.10')
import gst

class Player(QObject):
	pipelines = [None] * 4;
	memsinks = [None] * 4;

	def __init__(self, parent = None):
		super(Player, self).__init__(parent)

		for video_id in range(0,4):
			self.pipelines[video_id] = gst.parse_launch('rtspsrc location="rtsp://localhost:8554/camera_1%d" ! decodebin ! mfw_ipucsc ! memsink name=memsink%d' % video_id, video_id)
			self.memsinks[video_id] = self.pipelines[video_id].get_by_name('memsink%d' % video_id)

	@pyqtSlot(int, str)
	def memoryAddressChanged(self, video_id, address):
		self.memsinks[video_id].set_property('address', int(address))
		self.memsinks[video_id].set_property('memory-size', 352 * 288 * 4)
		self.pipelines[video_id].set_state(gst.STATE_PLAYING)


app = QGuiApplication(sys.argv)

view = QQuickView()
view.engine().quit.connect(app.quit)
view.setSource(QUrl('main.qml'))
view.setResizeMode(QQuickView.SizeRootObjectToView)

player = Player()
root = view.rootObject()
root.memoryAddressChanged.connect(player.memoryAddressChanged)

timer = QTimer()
timer.timeout.connect(view.update)
timer.start(30)

view.show()
sys.exit(app.exec_())
