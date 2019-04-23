defmodule LongestWordFinder.MixProject do
  use Mix.Project

  def project do
    [
      app: :longest_word_finder,
      version: "0.1.0",
      elixir: "~> 1.7",
      escript: escript(),
      start_permanent: Mix.env() == :prod,
      deps: deps()
    ]
  end

  # Run "mix help compile.app" to learn about applications.
  def application do
    [
      extra_applications: [:logger],
      mod: {LongestWordFinder.Application, []}
    ]
  end

  # Run "mix help deps" to learn about dependencies.
  defp deps do
    [
      {:flow, "~> 0.14"},
    ]
  end

  defp escript do
    [main_module: LongestWordFinder.CLI]
  end
end
