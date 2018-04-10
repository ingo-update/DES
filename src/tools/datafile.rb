ARGV.each do |name|
  puts "File #{name}:\n"
  File.open(name) do |file|
    file.each_line{ |line| puts line}
  end
  puts "\n"
end
