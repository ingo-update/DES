# Assume all arguments are filenames, output the name and the content.

ARGV.each do |name|
  puts "File #{File.basename(name)}:\n"
  File.open(name) do |file|
    file.each_line{ |line| puts line }
  end
  puts "\n"
end
