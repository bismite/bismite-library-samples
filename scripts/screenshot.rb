#!/bin/env ruby
require "fileutils"
FileUtils.mkdir_p "doc"
FileUtils.mkdir_p "tmp"

KEY = ('a'..'z').to_a

SOURCES = Dir.glob("src/*.c")
#SOURCES = [ "src/layer-particle.c" ]
#SOURCES = [ "src/hit.c" ]

SOURCES.each do |src|
  window_name = src
  name = File.basename src,".c"
  pid = spawn "./build/linux/#{name}"
  sleep 1

  xwininfo = `xwininfo -name "#{window_name}"`
  x = /Absolute upper-left X:\W+(\d+)/.match(xwininfo)[1]
  y = /Absolute upper-left Y:\W+(\d+)/.match(xwininfo)[1]

  `xdotool mousemove #{x} #{y}`
  `rm tmp/*.png`
  10.times do |pos|
   `xdotool key #{KEY.sample}`
   `xdotool mousemove_relative --sync 20 10 click 1`
    sleep 0.3
   `scrot -uz tmp/ss.png`
  end

  `convert -delay 30 tmp/*.png -colors 32 doc/#{name}.gif`
  Process.kill "HUP", pid
end
