@everywhere function euclideanDistanceSquaredM(v1::Vector{UInt8}, v2::Vector{UInt8})
  sum = 0
  for i in 1:length(v1)
    sum += (v1[i] - v2[i]) * (v1[i] - v2[i])
  end
	return sum
end

@everywhere function euclideanDistanceSquaredFloats(v1::Vector{Float16}, v2::Vector{Float16})
  sum = 0
  for i in 1:length(v1)
    sum += (v1[i] - v2[i]) * (v1[i] - v2[i])
  end
	return sum
end

@everywhere function randomMatrixGaussian(n::Int64, d::Int64)
  W = zeros(Float16, n, d)
  distr = Normal(0, sqrt(1/n))
  for i in 1:n
    for j in 1:d
      W[i,j] = rand(distr, 1)[1]
    end
  end
  return W
end

@everywhere function randomMatrixAchiloptas(n::Int64, d::Int64)
  W = zeros(Int8, n, d)
  values = [1,0,0,0,0,-1]
  for i in 1:n
    for j in 1:d
      W[i,j] = rand(values, 1)[1]
    end
  end
  return W
end

@everywhere function matrixMultiplicationGaussian(M1::Array{Float16, 2}, M2::Matrix{UInt8})
  nrows1 = size(M1)[1]
  nrows2 = size(M2)[1]
  ncols1 = size(M1)[2]
  ncols2 = size(M2)[2]
  M = zeros(nrows1, ncols2)
  for i in 1:nrows1
    for j in 1:ncols2
      soma = 0
      for k in 1:ncols1
        soma += M1[i,k] * M2[k,j]
      end
      M[i,j] = soma
    end
  end
  return M
end

@everywhere function matrixMultiplicationAchiloptas(M1::Array{Int8, 2}, M2::Matrix{UInt8})
  nrows1 = size(M1)[1]
  nrows2 = size(M2)[1]
  ncols1 = size(M1)[2]
  ncols2 = size(M2)[2]
  M = zeros(nrows1, ncols2)
  for i in 1:nrows1
    for j in 1:ncols2
      soma = 0
      for k in 1:ncols1
        soma += M1[i,k] * M2[k,j]
      end
      M[i,j] = soma
    end
  end
  return M
end
