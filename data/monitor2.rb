#!/usr/bin/env ruby

require 'set'
require 'pp'
require 'json'
require 'gnuplot'
class Gnuplotter

def plot( filename)
  linestyle=1
            Gnuplot.open do |gp|
                Gnuplot::Plot.new( gp ) do |plot|
                    plot.timefmt "\"%s\""
                    plot.title "QualityOverTime"
                    plot.ylabel "Value"
                    plot.xlabel "Iteration"
                   # plot.xdata "time"
                   # plot.format "x \"%m/%d/%Y %H:%M:%S\""
                    plot.format "y \"10^{%L}\""
                    plot.key "outside"
                    plot.key "right top"
                    plot.terminal "svg size 1920,1080 "
                    plot.output "qot.svg"
                    plot.object "1 rect from screen 0, 0, 0 to screen 1, 1, 0 behind"
                    plot. object " 1 rect fc  rgb 'white'  fillstyle solid 1.0"
                    plot.decimalsign "\',\'"
                    plot.style "line 12 lc rgb '#808080' lt 0 lw 1"
                    plot.grid "back ls 12"
                    plot.style "line 11 lc rgb '#808080' lt 1"
                    plot.border "3 back ls 11"
                    plot.tics "nomirror"
                    plot.style "line 1 lt 1 lc rgb '#D03C0D'  lw 2 pt 7 ps 1.5"
                    plot.style "line 2 lt 1 lc rgb '#00A000' lw 2 pt 11 ps 1.5"
                    plot.style "line 3 lt 1 lc rgb '#5060D0' lw 2 pt 9 ps 1.5"
                    plot.style "line 4 lt 1 lc rgb '#0000A0' lw 2 pt 8 ps 1.5"
                    plot.style "line 5 lt 1 lc rgb '#D0D000' lw 2 pt 13 ps 1.5"
                    plot.style "line 6 lt 1 lc rgb '#00D0D0' lw 2 pt 12 ps 1.5"
                    plot.style "line 7 lt 1 lc rgb '#B200B2' lw 2 pt 5 ps 1.5"
                    plot.logscale "y"
                    plot.arbitrary_lines<<"set tics font \",20\" "
                    plot.arbitrary_lines<<"set key font \",15\""
                    plot.arbitrary_lines<<"set ylabel font \",20\""
                    plot.arbitrary_lines<<"set xlabel font \",20\""
                    plot.arbitrary_lines<<"set title font \",20\""
#                    plot.style "data linespoints ls 1"
              filename.each{|name|
                puts"plotting", name;
                dataX=[];
                dataY=[];
                mygname=File.open(name).to_a.first.to_s;
                pp mygname
                File.open(name).drop(1).each{|line|

                    linearray=line.split()
                    dataX<< linearray[1]
                    dataY<< linearray[6]
                }
                plot.data<< Gnuplot::DataSet.new( [dataX, dataY] ) { |ds|
                  ds.with ="linespoints ls #{linestyle}"
                  ds.title="#{mygname.chomp()}"
                  ds.using='1:2'
                }
                linestyle+=1
              }
              end
            end
                #interval=(max-min)/5
                 #   plot.xrange "[\"#{min}\" :\"#{max}\" ]"
                  #  plot.xtics "\"#{min}\", #{interval} ,\"#{max}\""

 end
 end


monitor=Gnuplotter.new
monitor.plot(ARGV)
