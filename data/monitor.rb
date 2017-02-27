#!/usr/bin/env ruby

require 'set'
require 'pp'
require 'json'
require 'gnuplot'
class Gnuplotter
    def plot(filename)
        linestyle = 1
        Gnuplot.open do |gp|
            Gnuplot::Plot.new(gp) do |plot|
                plot.timefmt '"%s"'
                plot.title 'QualityOverTime'
                plot.ylabel 'Value'
                plot.xlabel 'Iteration'
                # plot.xdata "time"
                # plot.format "x \"%m/%d/%Y %H:%M:%S\""
                plot.format 'y "10^{%L}"'
                plot.key 'outside'
                plot.key 'right top'
                plot.terminal 'svg size 1920,1080 '
                plot.output 'qot.svg'
                plot.object '1 rect from screen 0, 0, 0 to screen 1, 1, 0 behind'
                plot. object " 1 rect fc  rgb 'white'  fillstyle solid 1.0"
                plot.decimalsign "\',\'"
                plot.style "line 12 lc rgb '#808080' lt 0 lw 1"
                plot.grid 'back ls 12'
                plot.style "line 11 lc rgb '#808080' lt 1"
                plot.border '3 back ls 11'
                plot.tics 'nomirror'
                plot.style "line 1 lt 1 lc rgb '#D03C0D'  lw 2 pt 7 ps 1.5"
                plot.style "line 2 lt 1 lc rgb '#00A000' lw 2 pt 11 ps 1.5"
                plot.style "line 3 lt 1 lc rgb '#5060D0' lw 2 pt 9 ps 1.5"
                plot.style "line 4 lt 1 lc rgb '#0000A0' lw 2 pt 8 ps 1.5"
                plot.style "line 5 lt 1 lc rgb '#D0D000' lw 2 pt 13 ps 1.5"
                plot.style "line 6 lt 1 lc rgb '#00D0D0' lw 2 pt 12 ps 1.5"
                plot.style "line 7 lt 1 lc rgb '#B200B2' lw 2 pt 5 ps 1.5"
                plot.logscale 'y'
                plot.arbitrary_lines << 'set tics font ",20" '
                plot.arbitrary_lines << 'set key font ",15"'
                plot.arbitrary_lines << 'set ylabel font ",20"'
                plot.arbitrary_lines << 'set xlabel font ",20"'
                plot.arbitrary_lines << 'set title font ",20"'
                #                    plot.style "data linespoints ls 1"

                set = []
                mset=[]
                i=1
                filename.each do |name|
                  puts"plotting:",name
                    filet = File.new(name, 'r')
                    dataX = []
                    dataY = []
                    filearr=filet.to_a
                    #myname=filearr.first
                    myname=i
                  i=i+1
                    fvalue= filearr.last.split().last

                    filearr.drop(1).each do |line|
                        linearray = line.split
                        dataX << linearray[1]
                        dataY << linearray.last
                    end
                        field=[myname,fvalue,dataX,dataY]

                    set << field

                end
                set.sort! { |a, b| a[1].to_f <=> b[1].to_f }

                set.each { |dax|
                    mname=dax[0].to_s
                 unless mset.include?mname then
                    puts mname
                    mset<<mname
                    dataX=[]
                    dataY=[]
                    dax[2].each{|g|dataY<<g};
                    dax[3].each{|g|dataX<<g};
                    plot.data << Gnuplot::DataSet.new( [dax[2],dax[3]] ) { |ds|
                        ds.with = "linespoints ls #{linestyle}"
                        ds.title = mname.chomp #title must not contain "\n"!
                        ds.using = '1:2'
                    }
                    linestyle += 1
                  end
               }
            end
        end
         # interval=(max-min)/5
         #   plot.xrange "[\"#{min}\" :\"#{max}\" ]"
         #  plot.xtics "\"#{min}\", #{interval} ,\"#{max}\""
     end
 end

monitor = Gnuplotter.new
monitor.plot(ARGV)
