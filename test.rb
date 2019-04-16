#!/usr/bin/env ruby
#

require 'thread'

# Convert text to an array to avoid UTF-8 complexity.
puts("Reading file.")
text = File.read('corpus.txt')
puts("done.")

def word_boundary?(arr, i) 
  case arr[i] 
  when "\r"
    true
  when "\n"
    true
  when "\t"
    true
  when " "
    true
  else
    false
  end
end

def time_this(&call)
  start_time = Time.now
  r = call.call()
  stop_time = Time.now
  puts("time #{(stop_time - start_time) * 1000}ms")
  r
end

def find_shortest(text, start, stop)
  i = start    # The index of the word start we are considering.
  loc = start  # The location of the longest work for final reporting.
  len = 1  # The length of the longest word.
  while i < stop do

    new_longest = true

    # Check backwards. If found before len, rewind to that and step.
    back_i = 0
    while back_i < len do
      if word_boundary?(text, i - back_i)
        new_longest = false
        break
      end
      back_i += 1
    end

    if new_longest
      loc = i - len + 1
      until word_boundary?(text, i)
        i += 1
      end
      len = i - loc
    else
      i = i - back_i + len
    end
  end
  [ len, text[loc...loc+len] ]
end

def find_shortest_threaded(text, thread_count)
  # Split the array.
  segment = (text.length / thread_count).ceil
  i = 0
  args = []
  while i < text.length do
    j = i + segment
    j = text.index(" ", j) || text.length

    args << [text, i, j]

    i = j
  end

  args.map do |args|
    Thread.new(args) do |a|
      find_shortest(*a)
    end
  end.reduce([0, '']) do |m, t|
    len, word = t.value
    if m[0] > len
      m
    else
      [ len, word ]
    end
  end

end


puts "Single thread."
len, word = time_this { find_shortest(text, 0, text.length) }
puts("longest #{len} \"#{word}\"")
len, word = time_this { find_shortest(text, 0, text.length) }
puts("longest #{len} \"#{word}\"")


puts "Multi-thread 8."
len, word = time_this { find_shortest_threaded(text, 8) }
puts("longest #{len} \"#{word}\"")
len, word = time_this { find_shortest_threaded(text, 8) }
puts("longest #{len} \"#{word}\"")

puts "Multi-thread 16."
len, word = time_this { find_shortest_threaded(text, 8) }
puts("longest #{len} \"#{word}\"")
len, word = time_this { find_shortest_threaded(text, 8) }
puts("longest #{len} \"#{word}\"")
