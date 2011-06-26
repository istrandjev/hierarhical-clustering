#f = File.open("test.txt","w")

num_classes = 5

class Point 
	attr_accessor :x, :y
	def initialize x, y
		@x = x
		@y = y
	end
	def to_s
		@x.to_s[0, 5] + " " + @y.to_s[0,5]
	end
end
class SphereClass
	attr_accessor :center_x
	attr_accessor :center_y
	attr_accessor :r
	attr_accessor :points
	def initialize size
		@center_x = 1.0 - 2.0*rand()
		@center_y = 1.0 - 2.0*rand()
		@r = 0.1 + rand()*0.2
		@points = []
		size.times do 
			@points << Point.new(@center_x + @r - 2.0*rand()*@r, @center_y - 2.0*rand()*@r)
		end
	end
end
class Array
  def shuffle!
    size.downto(1) { |n| push delete_at(rand(n)) }
    self
  end
end
classes = []
(0...num_classes).each do |i|
	size = (rand()*200).to_i;
	classes << SphereClass.new(size)  
end

all_points = []
(0...classes.size).each do |i|
	classes[i].points.each do |p|
		all_points << [p, i]
	end
end

all_points.shuffle!

File.open("test.txt","w") do |f|
	f.puts "2 #{all_points.size}"
	all_points.each{|p| f.puts p[0]}
end
puts all_points.inspect
cl = []
classes.size.times do 
	cl << []
end

(0...all_points.size).each do |i|
	cl[all_points[i][1]] << i
end

puts cl.inspect
File.open("classes.txt","w") do |f|
	f.puts classes.size
	(0...cl.size).each do |t|
		f.puts cl[t].size
		s = ""
		cl[t].each{|w| s += "#{w} "}
		s.strip!
		f.puts s
	end
end