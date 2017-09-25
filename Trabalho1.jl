#Pkg.add("JLD")
using Distributions
using DataFrames
using JLD
cd("C:\\Users\\Thiago\\Desktop\\Mestrado\\Algoritmos para Data Science (Laber)\\Trab1")
addprocs()
@everywhere using Distributions
@everywhere using DataFrames
@everywhere using JLD
pwd()
@everywhere cd("C:\\Users\\Thiago\\Desktop\\Mestrado\\Algoritmos para Data Science (Laber)\\Trab1")
@everywhere include("auxiliaresTrab1.jl")
@everywhere function main()
  #M = rand(UInt8, 3000, 40000)
  #cd("C:\\Users\\Thiago\\Desktop\\Mestrado\\Algoritmos para Data Science (Laber)\\Trab1")
  println("Vai comecar a ler o csv")
  Maux = readtable("dataset.csv");
  println("Acabou de ler o csv")
  numRows = size(Maux)[1];
  numCols = size(Maux)[2];
  Maux2 = Maux[1:numRows, 2:(numCols-1)];
  Maux3 = convert(Array, Maux2);
  MUInt8 = convert(Array{UInt8, 2}, Maux3);
  M = transpose(MUInt8)

  println("Gerou a matriz M")
#  dists = zeros(size(M)[1], size(M)[1])#
  #for i in 1:size(M)[1]
#    for j in i:size(M)[1]
      #dists[i,j] = euclideanDistanceSquaredM(M[i,:],M[j,:])
    #end
    #print(i)
  #end
  #print(toc())
  d = size(M)[1]
  N = size(M)[2]
  n_values = [4, 16, 64, 256, 1024, 4096, 4^floor(log(4, N))]
  println("Vai entrar no loop paralelizado")
  @parallel for i in 1:3
    println("Entrou no loop")
    for n in n_values[1:3]
      n = convert(Int64, n)
      println("n = ", n)
      #n=1
      internalCounter = 0
      println("Vai comecar a criar a Wgauss")
      tic(); W_gauss = randomMatrixGaussian(n, d);
      tempoCriacaoWGauss = toc()

      println("Vai comecar a criar a Wach")
      tic(); W_achiloptas = randomMatrixAchiloptas(n, d);
      tempoCriacaoWAch = toc();

      println("Comecando multiplicacao Gauss")
      tic(); M_gaussian = matrixMultiplicationGaussian(W_gauss, M);
      tempoMultiplicacaoGauss = toc();
      println("Comecando multiplicacao Ach")
      tic(); M_achiloptas = matrixMultiplicationAchiloptas(W_achiloptas, (M)) * sqrt(3/n);
      tempoMultiplicacaoAch = toc();

      M_gaussian = convert(Array{Float16, 2}, M_gaussian)
      M_achiloptas = convert(Array{Float16, 2}, M_achiloptas)

      tic();
      distsGaussian = zeros(size(M_gaussian)[1], size(M_gaussian)[1])#
      for i in 1:size(M_gaussian)[1]
        for j in i:size(M_gaussian)[1]
          distsGaussian[i,j] = euclideanDistanceSquaredFloats(M_gaussian[i,:],M_gaussian[j,:])
        end
        print(i)
      end
      tempoDistanciasGauss = toc();

      tic();
      distsAchiloptas = zeros(size(M_achiloptas)[1], size(M_achiloptas)[1])#
      for i in 1:size(distsAchiloptas)[1]
        for j in i:size(distsAchiloptas)[1]
          distsAchiloptas[i,j] = euclideanDistanceSquaredFloats(M_achiloptas[i,:],M_achiloptas[j,:])
        end
        print(i)
      end
      tic()
      tempoDistanciasAch = toc();
      JLD.save(string("resultados_n_",n,"_worker_",i,".jld"),
          "tempoCriacaoWGauss", tempoCriacaoWGauss,
          "tempoCriacaoWAch", tempoCriacaoWAch,
          "tempoMultiplicacaoGauss", tempoMultiplicacaoGauss,
          "tempoMultiplicacaoAch", tempoMultiplicacaoAch,
          "tempoDistanciasGauss", tempoDistanciasGauss,
          "tempoDistanciasAch", tempoDistanciasAch,
          "W_gauss", W_gauss,
          "W_achiloptas", W_achiloptas,
          "M_gaussian", M_gaussian,
          "M_achiloptas", M_achiloptas,
          "distsGaussian", distsGaussian,
          "distsAchiloptas", distsAchiloptas
      )
    end
  end
end

main()
