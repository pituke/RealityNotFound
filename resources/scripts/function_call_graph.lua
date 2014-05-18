local artifactsExtractor = require "luadb.extraction.extractor"
local lfs                = require "lfs"
local utils              = require "luadb.utils"

local graph = {}

local function inc()
  a = a or 0
  a = a + 1
  return a
end

local function extractNode(v, nodes, minComplexity, maxComplexity, minLines, maxLines)
  local origname = v.data.name or v.id
  local newnode = {type = "node", id = inc(), label = origname, params={size = 8, name = origname, origid = v.id, type = v.data.type, path = v.data.path, modulePath = v.data.modulePath, colorA = 1, colorR = 1, colorG = 1, colorB = 1}}
  if newnode.id == 1 then newnode.params.root = true end
  nodes[v] = newnode
  if v.data.type == 'file' or v.data.type == 'globalModule' then 
    newnode.params.colorA = 1
    newnode.params.colorR = 1
    newnode.params.colorG = 0
    newnode.params.colorB = 1
  end

  if v.data.type == 'directory' then 
    newnode.params.colorA = 1
    newnode.params.colorR = 0
    newnode.params.colorG = 1
    newnode.params.colorB = 1
  end

  if v.data.type == 'globalFunction' then 
    newnode.params.colorA = 1
    newnode.params.colorR = 0
    newnode.params.colorG = 0
    newnode.params.colorB = 1
  end
 
  if v.data.type == 'function' then
    newnode.params.tag = v.data.tag       
    if v.data.metrics ~= nil then
      newnode.params.metrics = {}
      newnode.params.metrics.halstead = v.data.metrics.halstead
      newnode.params.metrics.cyclomatic = v.data.metrics.cyclomatic
      newnode.params.metrics.LOC = v.data.metrics.LOC
      newnode.params.metrics.infoflow = v.data.metrics.infoflow

      minComplexity = (minComplexity and (minComplexity < newnode.params.metrics.cyclomatic.upperBound and minComplexity)) or newnode.params.metrics.cyclomatic.upperBound
      maxComplexity = (maxComplexity and (maxComplexity > newnode.params.metrics.cyclomatic.upperBound and maxComplexity)) or newnode.params.metrics.cyclomatic.upperBound
      minLines = (minLines and (minLines < newnode.params.metrics.LOC.lines and minLines)) or newnode.params.metrics.LOC.lines
      maxLines = (maxLines and (maxLines > newnode.params.metrics.LOC.lines and maxLines)) or newnode.params.metrics.LOC.lines
    end
  end
  return minComplexity, maxComplexity, minLines, maxLines
end

local function setEdgeColor(edge)
  if edge.params.type == 'function call' then
    edge.params.colorA = 1
    edge.params.colorR = 0.8
    edge.params.colorG = 0.8
    edge.params.colorB = 1
  elseif edge.params.type == 'in file' then
    edge.params.colorA = 1
    edge.params.colorR = 1
    edge.params.colorG = 0.8
    edge.params.colorB = 0.8
  elseif edge.params.type == 'in directory' then
    edge.params.colorA = 1
    edge.params.colorR = 0.8
    edge.params.colorG = 1
    edge.params.colorB = 0.8
  end
end

local function extractEdge(v, existingedges, nodes)
  if #v.from ~= 1 then print('from', #v.from, v.id, v.from[1], v.from[2]) end 
  if #v.to ~= 1 then print('to', #v.to, v.id, v.to[1], v.to[2]) end    
  local ind = v.from[1].id .. "|" .. v.to[1].id
  if existingedges[ind] ~= nil then
    existingedges[ind].params.count = existingedges[ind].params.count + 1
  else
    local edge = {type = "edge", id = inc(), label = '', params = {origid = v.id, count = 1, edgeStrength = 2}}
    local incid1 = {type = "edge_part", id = inc(), label = ''}
    local incid2 = {type = "edge_part", id = inc(), label = ''}
    if v.from[1].data.type == 'function' or v.to[1].data.type == 'globalFunction' then
      edge.params.type = "function call"
      incid1.direction = 'in'
      incid2.direction = 'out'
    end
    if v.from[1].data.type == 'file' then 
      edge.params.type = 'in file'
    end
    if v.from[1].data.type == 'directory' then 
      edge.params.type = 'in directory'
    end
    if v.from[1].data.modulePath ~= v.to[1].data.modulePath then
      edge.params.edgeStrength = 0.1
    elseif v.from[1].data.modulePath ~= nil then
      edge.params.edgeStrength = 0.5
    end 
    graph[edge] = {[incid1] = nodes[v.from[1]], [incid2] = nodes[v.to[1]]}
    existingedges[ind] = edge
    setEdgeColor(edge)
  end
end

local function doVisualMapping(nodes, minComplexity, maxComplexity, minLines, maxLines)
  local minSize = 8
  local maxSize = 100
  for k, n in pairs(nodes) do
    if n.params.type == 'function' then
      if n.params.metrics ~= nil then
        n.params.size = minSize + (n.params.metrics.LOC.lines - minLines) / (maxLines - minLines) * (maxSize - minSize)
        local complexRatio = (n.params.metrics.cyclomatic.upperBound - minComplexity) / (maxComplexity - minComplexity) 
        n.params.colorG = 1 - complexRatio
        n.params.colorR = complexRatio
        n.params.colorB = 0
      else
        n.params.colorG = 0
        n.params.colorR = 0
        n.params.colorB = 1
      end
    end
  end
end

local function extractGraph(absolutePath)
  graph = {}
  utils.logger:setLevel(utils.logging.INFO)

  utils.logger:info("started extraction")
  local extractedGraph = artifactsExtractor.extract(absolutePath)
  utils.logger:info("extraction successfully finished")

  extractedGraph:printNodes()
  extractedGraph:printEdges()

  local nodes = {}
  local minComplexity, maxComplexity
  local minLines, maxLines

  for _, v in ipairs(extractedGraph.nodes) do
    minComplexity, maxComplexity, minLines, maxLines = extractNode(v, nodes, minComplexity, maxComplexity, minLines, maxLines)
  end

  print("complexity", minComplexity, maxComplexity, minLines, maxLines)

  local existingedges = {}

  for _, v in ipairs(extractedGraph.edges) do
    extractEdge(v, existingedges, nodes)
  end
  
  doVisualMapping(nodes, minComplexity, maxComplexity, minLines, maxLines)
end

local function getGraph()
  print"getting function graph"
  return graph
end

return {extractGraph = extractGraph,
  getGraph = getGraph}
