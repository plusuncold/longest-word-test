defmodule LongestWordFinder.CLI do
  def main(args \\ []) do
    args
    |> hd()
    |> String.trim()
    |> command()
    |> IO.puts()
  end

  defp command(arg) when arg in ["single_threaded", "concurrent"] do
    LongestWordFinder.result(arg)
  end

  defp command(_arg) do
    "Invalid argument"
  end
end
