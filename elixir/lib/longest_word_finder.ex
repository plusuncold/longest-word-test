defmodule LongestWordFinder do
  def concurrent() do
    load_corpus()
    |> Flow.from_enumerable()
    |> Flow.map(&String.trim_trailing/1)
    |> Flow.flat_map(&(String.split(&1, " ")))
    |> Flow.partition()
    |> Flow.reduce(fn -> %{word: "", length: 0} end, &longest_word/2)
    |> Enum.into(%{})
  end

  def single_threaded() do
    load_corpus()
    |> Stream.map(&String.trim_trailing/1)
    |> Stream.flat_map(&(String.split(&1, " ")))
    |> Enum.reduce(%{word: "", length: 0}, &longest_word/2)
  end

  def result("single_threaded") do
    %{word: word, length: length} = single_threaded()
    "[Single Threaded] word: #{word}, length: #{length}"
  end

  def result("concurrent") do
    %{word: word, length: length} = concurrent()
    "[Concurrent] word: #{word}, length: #{length}"
  end

  def result(_) do
    "Not a valid option"
  end

  def concurrent_timed_in_ms() do
    milliseconds(&concurrent/0)
  end

  def single_threaded_timed_in_ms() do
    milliseconds(&single_threaded/0)
  end

  defp milliseconds(fun) do
    fun
    |> :timer.tc()
    |> elem(0)
    |> Kernel.*(0.001)
    |> round()
  end

  defp load_corpus() do
    "../txt/corpus.txt"
    |> Path.expand(__DIR__)
    |> File.stream!()
  end

  defp longest_word(word, acc) do
    word_length = String.length(word)

    if word_length > acc.length do
      %{word: word, length: word_length}
    else
      acc
    end
  end
end
