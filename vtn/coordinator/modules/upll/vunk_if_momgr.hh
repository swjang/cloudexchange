/*
 * Copyright (c) 2012-2014 NEC Corporation
 * All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v1.0 which accompanies this
 * distribution, and is available at http://www.eclipse.org/legal/epl-v10.html
 */

#ifndef UNC_UPLL_VUNK_IF_MOMGR_H
#define UNC_UPLL_VUNK_IF_MOMGR_H

#include <string>
#include <list>
#include <set>
#include "momgr_impl.hh"
namespace unc {
namespace upll {
namespace kt_momgr {



class VunkIfMoMgr : public MoMgrImpl {
  private:
    static BindInfo vunk_if_bind_info[];
    static BindInfo key_vunk_if_maintbl_update_bind_info[];
    /**
     * @brief  Gets the valid array position of the variable in the value
     *         structure from the table in the specified configuration
     *
     * @param[in]     val      pointer to the value structure
     * @param[in]     indx     database index for the variable
     * @param[out]    valid    position of the variable in the valid array -
     *                          NULL if valid does not exist.
     * @param[in]     dt_type  specifies the configuration
     * @param[in]     tbl      specifies the table containing the given value
     *
     **/
    upll_rc_t GetValid(void *val,
                       uint64_t indx,
                       uint8_t *&valid,
                       upll_keytype_datatype_t dt_type,
                       MoMgrTables tbl) {
      if (val == NULL) return UPLL_RC_ERR_GENERIC;
      valid = NULL;
      if (tbl == MAINTBL) {
        switch (indx) {
          case uudst::vunknown_interface::kDbiAdminStatus:
            valid = &(reinterpret_cast<val_vunk_if *>(val))->
                          valid[UPLL_IDX_ADMIN_ST_VUNI];
            break;
          case uudst::vunknown_interface::kDbiDesc:
            valid = &(reinterpret_cast<val_vunk_if *>(val))->
                          valid[UPLL_IDX_DESC_VUNI];
            break;
          default:
            valid = NULL;
            return UPLL_RC_ERR_GENERIC;
        }
      }
      return UPLL_RC_SUCCESS;
    }
    upll_rc_t UpdateConfigStatus(ConfigKeyVal *req, unc_keytype_operation_t op,
                                 uint32_t driver_result, ConfigKeyVal *upd_key,
                                 DalDmlIntf *dmi,
                                 ConfigKeyVal *ctrlr_key = NULL);
    /**
     * @brief  Update config status for commit result and vote result.
     *
     * @param[in/out]  ckv_running  ConfigKeyVal instance.
     * @param[in]      cs_status    either UNC_CS_INVALID or UNC_CS_APPLIED.
     * @param[in]      phase        specify the phase (CREATE, DELETE or UPDATE)
     * @param[in]      dmi          Pointer to the DalDmlIntf(DB Interface)
     *
     **/
    upll_rc_t UpdateAuditConfigStatus(unc_keytype_configstatus_t cs_status,
                                      uuc::UpdateCtrlrPhase phase,
                                      ConfigKeyVal *&ckv_running,
                                      DalDmlIntf *dmi);

    /**
     * @brief  Compares the valid value between two database records.
     * 	    if both the values are same, update the valid flag for corresponding
     * 	     attribute as invalid in the first record.
     *
     * @param[in/out]  val1   first record value instance.
     * @param[in]      val2   second record value instance.
     * @param[in]      auditi if true, CompareValidValue called from audit
     *                         process.
     *
     **/
    bool CompareValidValue(void *&val1,
                           void *val,
                           bool audit);

    /**
     * @Brief Validates the syntax of the specified key and value structure
     *        for KT_VUNK_IF keytype
     *
     * @param[in] req                       This structure contains
     *                                      IpcReqRespHeader(first 8 fields of
     *                                      input request structure).
     * @param[in] ikey                    ikey contains key and value structure.
     *
     * @retval UPLL_RC_SUCCESS              Successful.
     * @retval UPLL_RC_ERR_CFG_SYNTAX       Syntax error.
     * @retval UPLL_RC_ERR_NO_SUCH_INSTANCE key_vlink is not available.
     * @retval UPLL_RC_ERR_GENERIC          Generic failure.
     * @retval UPLL_RC_ERR_INVALID_OPTION1  option1 is not valid.
     * @retval UPLL_RC_ERR_INVALID_OPTION2  option2 is not valid.
     */
    upll_rc_t ValidateMessage(IpcReqRespHeader *req,
                              ConfigKeyVal *ikey);

    /**
     * @Brief Validates the syntax for KT_VUNK_IF keytype key structure.
     *
     * @param[in] key_vunk_if KT_VUNK_IF key structure.
     * @param[in] operation    Operation type.
     *
     * @retval UPLL_RC_SUCCESS        validation succeeded.
     * @retval UPLL_RC_ERR_CFG_SYNTAX validation failed.
     */
    upll_rc_t ValidateVunkIfKey(key_vunk_if_t *key_vunk_if,
        unc_keytype_operation_t operation = UNC_OP_INVALID);

    /**
     * @Brief Validates the syntax for KT_VUNK_IF keytype value structure.
     *
     * @param[in] val_vlink KT_VUNK_IF value structure.
     *
     * @retval UPLL_RC_ERR_GENERIC    Generic failure.
     * @retval UPLL_RC_SUCCESS        validation succeeded.
     * @retval UPLL_RC_ERR_CFG_SYNTAX validation failed.
     */
    upll_rc_t ValidateVunkIfValue(val_vunk_if_t *val_vunk_if,
        unc_keytype_operation_t operation = UNC_OP_INVALID);

    /**
     * @Brief Validates the syntax for KT_VUNK_IF keytype value structure.
     *
     * @param[in] val_vtn_neighbor KT_VUNK_IF value structure.
     *
     * @retval UPLL_RC_ERR_GENERIC    Generic failure.
     * @retval UPLL_RC_SUCCESS        validation succeeded.
     * @retval UPLL_RC_ERR_CFG_SYNTAX validation failed.
     */
    upll_rc_t ValidateVtnNeighValue(val_vtn_neighbor_t *val_vtn_neighbor);

    /**
     * @Brief Checks if the specified key type(KT_VUNK_IF) and
     *        associated attributes are supported on the given controller,
     *        based on the valid flag
     *
     * @param[in] req               This structure contains
     *                              IpcReqRespHeader(first 8 fields of input
     *                              request structure).
     * @param[in] ikey              ikey contains key and value structure.
     *
     * @retval  UPLL_RC_SUCCESS             Validation succeeded.
     * @retval  UPLL_RC_ERR_GENERIC         Validation failure.
     * @retval  UPLL_RC_ERR_INVALID_OPTION1 Option1 is not valid.
     * @retval  UPLL_RC_ERR_INVALID_OPTION2 Option2 is not valid.
     */
    upll_rc_t ValidateCapability(IpcReqRespHeader *req,
                                 ConfigKeyVal *ikey,
                                 const char *ctrlr_name = NULL) {
      return UPLL_RC_SUCCESS;
    }
    /**
     * @brief  Duplicates the input configkeyval including the key and val.
     * based on the tbl specified.
     *
     * @param[in]  okey   Output Configkeyval - allocated within the function
     * @param[in]  req    Input ConfigKeyVal to be duplicated.
     * @param[in]  tbl    specifies if the val structure belongs to the main
     *                      table/ controller table or rename table.
     *
     * @retval         UPLL_RC_SUCCESS      Successfull completion.
     * @retval         UPLL_RC_ERR_GENERIC  Failure case.
     **/
    upll_rc_t DupConfigKeyVal(ConfigKeyVal *&okey,
                              ConfigKeyVal *&req,
                              MoMgrTables tbl = MAINTBL);
    /**
     * @brief  Allocates for the specified val in the given configuration in the
     *                   specified table.
     *
     * @param[in/out]  ck_val   Reference pointer to configval structure
     *                          allocated.
     * @param[in]      dt_type  specifies the configuration candidate/running/
     *                          state
     * @param[in]      tbl      specifies if the corresponding table is the
     *                          main table / controller table or rename table.
     *
     * @retval         UPLL_RC_SUCCESS      Successfull completion.
     * @retval         UPLL_RC_ERR_GENERIC  Failure case.
     **/
    upll_rc_t AllocVal(ConfigVal *&ck_val,
                       upll_keytype_datatype_t dt_type,
                       MoMgrTables tbl = MAINTBL);
    /**
     * @brief      Method to get a configkeyval of a specified keytype from an
     *         input configkeyval
     *
     * @param[in/out]  okey                 pointer to output ConfigKeyVal
     * @param[in]      parent_key           pointer to the configkeyval from
     *                            which the output configkey val is initialized.
     *
     * @retval         UPLL_RC_SUCCESS      Successfull completion.
     * @retval         UPLL_RC_ERR_GENERIC  Failure case.
     */
    upll_rc_t GetChildConfigKey(ConfigKeyVal *&okey,
                                ConfigKeyVal *parent_key);
    /**
     * @brief      Method to get a configkeyval of the parent keytype
     *
     * @param[in/out]  okey           pointer to parent ConfigKeyVal
     * @param[in]      ikey           pointer to the child configkeyval from
     * which the parent configkey val is obtained.
     *
     * @retval         UPLL_RC_SUCCESS      Successfull completion.
     * @retval         UPLL_RC_ERR_GENERIC  Failure case.
     **/
    upll_rc_t GetParentConfigKey(ConfigKeyVal *&okey,
                                 ConfigKeyVal *ikey);
    upll_rc_t GetRenamedControllerKey(ConfigKeyVal *ikey,
                                      upll_keytype_datatype_t dt_type,
                                      DalDmlIntf *dmi,
                                      uint8_t *ctrlr_name) {
      return UPLL_RC_ERR_NOT_ALLOWED_FOR_THIS_KT;
    }
    upll_rc_t GetRenamedUncKey(ConfigKeyVal *ctrlr_key,
                               upll_keytype_datatype_t dt_type,
                               DalDmlIntf *dmi,
                               uint8_t *ctrlr_id) {
      return UPLL_RC_ERR_NOT_ALLOWED_FOR_THIS_KT;
    }
    /* Rename */
    upll_rc_t CopyToConfigKey(ConfigKeyVal *&okey,
                              ConfigKeyVal *ikey);
    bool GetRenameKeyBindInfo(unc_key_type_t key_type,
                              BindInfo *&binfo,
                              int &nattr,
                              MoMgrTables tbl);
    upll_rc_t ValidateAttribute(ConfigKeyVal *kval,
                                DalDmlIntf *dmi,
                                IpcReqRespHeader *req = NULL) {
      // string s(kval->ToStrAll());
      // UPLL_LOG_DEBUG(" %s", s.c_str());
      return UPLL_RC_SUCCESS;
    }

  public:
    VunkIfMoMgr();
    virtual ~VunkIfMoMgr() {
      for (int i = 0; i < ntable; i++)
        if (table[i]) {
          delete table[i];
        }
      delete[] table;
    }
    /**
     * @brief      Method to check if individual portions of a key are valid
     *
     * @param[in/out]  ikey                pointer to ConfigKeyVal referring to
     *                                     a UNC resource
     * @param[in]      index               db index associated with the variable
     *
     * @retval         true                 input key is valid
     * @retval         false                input key is invalid.
     **/
    bool IsValidKey(void *key,
                    uint64_t index);
    virtual upll_rc_t TxVoteCtrlrStatus(unc_key_type_t keytype,
                                      list<CtrlrVoteStatus*> *ctrlr_vote_status,
                                      DalDmlIntf *dmi) {
      return UPLL_RC_SUCCESS;
    }
    virtual upll_rc_t TxUpdateController(unc_key_type_t keytype,
                                         uint32_t session_id,
                                         uint32_t config_id,
                                         uuc::UpdateCtrlrPhase phase,
                                         set<string> *affected_ctrlr_set,
                                         DalDmlIntf *dmi,
                                         ConfigKeyVal **err_ckv) {
      return UPLL_RC_SUCCESS;
    }
    virtual upll_rc_t MergeImportToCandidate(unc_key_type_t keytype,
                                             const char *ctrlr_id,
                                             DalDmlIntf *dmi) {
      return UPLL_RC_SUCCESS;
    }
    virtual upll_rc_t ImportClear(unc_key_type_t keytype, const char *ctrlr_id,
                                  DalDmlIntf *dmi) {
      return UPLL_RC_SUCCESS;
    }
    virtual upll_rc_t AuditUpdateController(unc_key_type_t keytype,
                                            const char *ctrlr_id,
                                            uint32_t session_id,
                                            uint32_t config_id,
                                            uuc::UpdateCtrlrPhase phase,
                                            DalDmlIntf *dmi,
                                            ConfigKeyVal **err_ckv,
                                            KTxCtrlrAffectedState *ctrlr_affected) {
      return UPLL_RC_SUCCESS;
    }
    virtual upll_rc_t AuditVoteCtrlrStatus(unc_key_type_t keytype,
                                           CtrlrVoteStatus *vote_satus,
                                           DalDmlIntf *dmi) {
      return UPLL_RC_SUCCESS;
    }
    virtual upll_rc_t AuditCommitCtrlrStatus(unc_key_type_t keytype,
                                             CtrlrCommitStatus *commit_satus,
                                             DalDmlIntf *dmi) {
      return UPLL_RC_SUCCESS;
    }
    virtual upll_rc_t AuditEnd(unc_key_type_t keytype, const char *ctrlr_id,
                               DalDmlIntf *dmi) {
      return UPLL_RC_SUCCESS;
    }

    /* @brief         Populate val_vtn_neighbor for the READ/READ_SIBLING
     *        operations
     *
     * @param[in/out] key   Pointer to the ConfigKeyVal Structure
     * @param[in]     dmi    Pointer to the DalDmlIntf(DB Interface)
     *
     * @retval  UPLL_RC_SUCCESS                    Completed successfully.
     * @retval  UPLL_RC_ERR_GENERIC                Generic failure.
     * @retval  UPLL_RC_ERR_RESOURCE_DISCONNECTED  Resource disconnected.
     * @retval  UPLL_RC_ERR_DB_ACCESS              DB Read/Write error.
     * @retval  UPLL_RC_ERR_NO_SUCH_INSTANCE       Given key does not exist
     *
     **/
    upll_rc_t PopulateValVtnNeighbor(ConfigKeyVal *&key, DalDmlIntf *dmi);

    upll_rc_t IsReferenced(ConfigKeyVal *ikey, upll_keytype_datatype_t dt_type,
                           DalDmlIntf *dmi);

    virtual upll_rc_t GetPortMap(ConfigKeyVal *ikey, uint8_t &valid_pm,
                                 val_port_map_t *&pm, uint8_t &valid_admin,
                                 uint8_t &admin_status) {
      UPLL_FUNC_TRACE;
      if (ikey == NULL) return UPLL_RC_ERR_GENERIC; 
      val_vunk_if *ifval = reinterpret_cast<val_vunk_if *>
                                                 (GetVal(ikey));
      if (!ifval) {
        UPLL_LOG_DEBUG("Invalid param");
        return UPLL_RC_ERR_GENERIC;
      }
      valid_pm = UNC_VF_INVALID;
      pm = NULL;
      valid_admin = ifval->valid[UPLL_IDX_ADMIN_ST_VUNI]; 
      admin_status = ifval->admin_status;
      return UPLL_RC_SUCCESS;
    }
};

}  // namespace kt_momgr
}  // namespace upll
}  // namespace unc
#endif
