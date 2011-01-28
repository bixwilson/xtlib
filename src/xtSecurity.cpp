/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2011 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */
/**
    \class xtSecurity
    \brief A security object for specifying the logical user and checking privnames.

    Used by the other various classes to query a security model that determines if a user
    does or does not have a named priviledge.

 */

#include <stdexcept>

#include "xtSecurity.h"
#include "xtDatabase.h"
#include "xtQuery.h"

//
// Internal data
//

static std::string _logicalUsername = "domain";


//
// xtSecurity class implementation
//

xtSecurity::xtSecurity()
{
}

xtSecurity::~xtSecurity()
{
}

bool xtSecurity::hasPriv(const std::string & privname)
{
  std::string sql = "SELECT priv_name"
                    "  FROM usrpriv, priv"
                    " WHERE((usrpriv_priv_id=priv_id)"
                    "   AND (usrpriv_username='";
  sql += xtDatabase::getInstance()->escapeString(_logicalUsername);
  sql += "')"
                    "   AND (priv_name='";
  sql += xtDatabase::getInstance()->escapeString(privname);
  sql += "'))"
                    " UNION "
                    "SELECT priv_name"
                    "  FROM priv, grppriv, usrgrp"
                    " WHERE((usrgrp_grp_id=grppriv_grp_id)"
                    "   AND (grppriv_priv_id=priv_id)"
                    "   AND (usrgrp_username='";
  sql += xtDatabase::getInstance()->escapeString(_logicalUsername);
  sql += "')"
                    "   AND (priv_name='";
  sql += xtDatabase::getInstance()->escapeString(privname);
  sql += "'));";

  xtQuery query;
  query.exec(sql);

  if(query.rowCount() < 1)
    return false;

  return true;
}

void xtSecurity::setLogicalUser(const std::string & username)
{
  _logicalUsername = username;
}

std::string xtSecurity::logicalUser()
{
  return _logicalUsername;
}
