#ifndef DLR_H
#define DLR_H

#include <graph/graph_runtime.h>

#include <dlpack/dlpack.h>
#include <dmlc/json.h>
#include <dmlc/memory_io.h>
#include <tvm/runtime/ndarray.h>
#include <tvm/runtime/packed_func.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

enum class DLRBackend {
  kTVM,
  kBAREMETAL
};

class BareMetal {
public:
  std::string include_section;
  std::string extern_section;
  std::string init_section;
  std::string main_section;
  std::string kernel;
};

namespace dlr {

class DLR {
public:
  void Build(const std::string &json, const std::string &params, const std::string &lib, const DLRBackend Backend);
  void dump_kernel();
  std::vector<int64_t> GetShpae(const std::string &name);
  int64_t GetShpaeSize(const std::string &name);
  int64_t GetShpaeSize(tvm::runtime::GraphRuntime::OpArgs &op, int p_num);
  void SetInputPtr(int index, char *data_in, std::vector<int> &shape);
  void SetInputPtr(int index, void *data_in, std::vector<int> &shape, DLDataType *setType);
  void SetInputPtr(int op, int p_num, void *data_in, DLDataType *setType);
  void GetOutputPtr(int op, int p_num, void **data_out, DLDataType **getType);
  void *GetOutputPtr(int index);
  void *GetOutputPtr(tvm::runtime::GraphRuntime::OpArgs &op, int p_num);

  // will be generated by dump_kernel()
  void Run();
  void InitOp();
  /*!
   * \brief Initialize the graph executor with graph and context.
   * \param graph_json The execution graph.
   * \param module The module containing the compiled functions for the host
   *  processor.
   * \param ctxs The context of the host and devices where graph nodes will be
   *  executed on.
   */

  void Init(const std::string &graph_json,
            tvm::runtime::Module module,
            const std::vector<TVMContext> &ctxs);

  /*!
   * \brief Get the input index given the name of input.
   * \param name The name of the input.
   * \return The index of input.
   */
  int GetInputIndex(const std::string &name);

  /*!
   * \brief set index-th input to the graph.
   * \param index The input index.
   * \param data_in The input data.
   */
  void SetInput(int index, DLTensor *data_in);
  /*!
   * \brief Get the number of outputs
   *
   * \return The number of outputs from graph.
   */
  int NumOutputs() const;
  /*!
   * \brief Return DLTensor for given name.
   * 
   * \return DLTensor.
   */
  DLTensor GetInput(const std::string &name);
  /*!
   * \brief Return NDArray for given output index.
   * \param index The output index.
   *
   * \return NDArray corresponding to given output node index.
   */
  tvm::runtime::NDArray GetOutput(int index) const;
  /*!
   * \brief Copy index-th output to data_out.
   * \param index The output index.
   * \param data_out the output data.
   */
  void CopyOutputTo(int index, DLTensor *data_out);
  /*!
   * \brief Load parameters from binary stream
   * \param strm The input stream.
   */
  void LoadParams(dmlc::Stream *strm);
  /*!
   * \brief Load parameters from parameter blob.
   * \param param_blob A binary blob of parameter.
   */
  void LoadParams(const std::string &param_blob);

  /*!
   * \brief Share parameters from pre-existing GraphRuntime instance.
   * \param other A GraphRuntime instance, previously with |LoadParams| called with the
   * identical input |param_blob|.
   * \param strm The input stream.
   */
  void ShareParams(const tvm::runtime::GraphRuntime &other, dmlc::Stream *strm);

  std::shared_ptr<tvm::runtime::GraphRuntime> tvm_graph_runtime_;

  void *backend_handle;

  struct op {
    void *values;
    void *tcodes;
    int num;
  };
  typedef struct op op;
  op *opa;

  /*! \brief The graph nodes. */
  //std::vector<Node> nodes_;
  /*! \brief The argument nodes. */
  //std::vector<uint32_t> input_nodes_;
  /*! \brief Used for quick entry indexing. */
  //std::vector<uint32_t> node_row_ptr_;
  /*! \brief Output entries. */
  //std::vector<NodeEntry> outputs_;
  /*! \brief Additional graph attributes. */
  //GraphAttr attrs_;
  /*! \brief The code module that contains both host and device code. */
  //tvm::runtime::Module module_;
  /*! \brief Execution context of all devices including the host. */
  //std::vector<TVMContext> ctxs_;
  /*! \brief Common storage pool for all devices. */
  //std::vector<NDArray> storage_pool_;
  /*! \brief Data entry of each node. */
  //std::vector<NDArray> data_entry_;
  /*! \brief Operator on each node. */
  //std::vector<std::function<void()> > op_execs_;
};

std::vector<TVMContext> GetAllContext(const tvm::runtime::TVMArgs &args);
} // namespace dlr

#endif // DLR_H
