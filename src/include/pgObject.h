//////////////////////////////////////////////////////////////////////////
//
// pgAdmin III - PostgreSQL Tools
// Copyright (C) 2002 - 2003, The pgAdmin Development Team
// This software is released under the Artistic Licence
//
// pgObject.h - PostgreSQL base object class
//
//////////////////////////////////////////////////////////////////////////

#ifndef PGOBJECT_H
#define PGOBJECT_H

// wxWindows headers
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include "ctlSQLBox.h"

// App headers
#include "pgAdmin3.h"
#include "misc.h"


class frmMain;
class pgDatabase;
class pgSchema;
class pgCollection;
class pgConn;
class pgSet;
class pgServer;

// This enum lists the type of objects that may be included in the treeview
// as objects. If changing, update typesList[] as well.
enum PG_OBJTYPE
{
    PG_NONE,
    PG_SERVERS,         PG_SERVER,
    PG_DATABASES,       PG_DATABASE,
    PG_GROUPS,          PG_GROUP,
    PG_USERS,           PG_USER,
    PG_LANGUAGES,       PG_LANGUAGE,
    PG_SCHEMAS,         PG_SCHEMA,
    PG_AGGREGATES,      PG_AGGREGATE,
    PG_CASTS,           PG_CAST,
    PG_CONVERSIONS,     PG_CONVERSION,
    PG_DOMAINS,         PG_DOMAIN,
    PG_FUNCTIONS,       PG_FUNCTION,
    PG_TRIGGERFUNCTIONS,PG_TRIGGERFUNCTION,
    PG_OPERATORS,       PG_OPERATOR,
    PG_OPERATORCLASSES, PG_OPERATORCLASS,
    PG_SEQUENCES,       PG_SEQUENCE,
    PG_TABLES,          PG_TABLE,
    PG_TYPES,           PG_TYPE,
    PG_VIEWS,           PG_VIEW,
    PG_COLUMNS,         PG_COLUMN,
    PG_INDEXES,         PG_INDEX,
    PG_RULES,           PG_RULE,
    PG_TRIGGERS,        PG_TRIGGER,

    PG_CONSTRAINTS,     PG_PRIMARYKEY, PG_UNIQUE, PG_CHECK, PG_FOREIGNKEY,
    
    PG_UNKNOWN
};

class pgTypes
{
public:
    wxChar *typName;
    wxChar *newString;
    wxChar *newLongString;
};


extern pgTypes typesList[];

// Class declarations
class pgObject : public wxTreeItemData
{
protected:
    pgObject(int newType, const wxString& newName);

public:

    static wxString GetPrivileges(const wxString& allPattern, const wxString& acl, const wxString& grantObject, const wxString& user);

    virtual void ShowProperties() const {};
    int GetType() const { return type; }
    wxString GetTypeName() const { return typesList[type].typName; }
    void iSetName(const wxString& newVal) { name = newVal; }
    wxString GetName() const { return name; }
    OID GetOid() const { return oid; }
    wxString GetOidStr() const {return NumToStr(oid) + wxT("::oid"); }
    void iSetOid(const OID newVal) { oid = newVal; } 
    wxString GetOwner() const { return owner; }
    void iSetOwner(const wxString& newVal) { owner = newVal; }
    wxString GetComment() const { return comment; }
    void iSetComment(const wxString& newVal) { comment = newVal; }
    wxString GetAcl() const { return acl; }
    void iSetAcl(const wxString& newVal) { acl = newVal; }
    virtual bool GetSystemObject() const { return false; }
    virtual bool IsCollection() const { return false; }

    void ShowTree(frmMain *form, wxTreeCtrl *browser, wxListCtrl *properties, wxListCtrl *statistics, ctlSQLBox *sqlPane);
    void AppendBrowserItem(wxTreeCtrl *browser, pgObject *object);

    virtual wxString GetHelpPage(bool forCreate) const;
    virtual wxString GetFullName() const { return name; }
    virtual wxString GetIdentifier() const { return name; }
    virtual wxString GetQuotedIdentifier() const { return qtIdent(name); }

    virtual wxMenu *GetNewMenu();
    virtual wxString GetSql(wxTreeCtrl *browser) { return wxT(""); }
    wxString GetGrant(const wxString& allPattern, const wxString& grantFor=wxT(""), bool noOwner=false);
    wxString GetCommentSql();
    pgDatabase *GetDatabase();
    pgConn *GetConnection();

    virtual void SetDirty() { sql=wxT(""); expandedKids=false; needReread=true; }
    virtual void SetSql(wxTreeCtrl *browser, ctlSQLBox *sqlPane, const int index) { return; }
    virtual wxString GetFullIdentifier() const { return GetName(); }
    virtual wxString GetQuotedFullIdentifier() const { return qtIdent(GetName()); }

    virtual void ShowTreeDetail(wxTreeCtrl *browser, frmMain *form=0, wxListCtrl *properties=0, wxListCtrl *statistics=0, ctlSQLBox *sqlPane=0)
        =0;
    virtual pgObject *Refresh(wxTreeCtrl *browser, const wxTreeItemId item) {return this; }
    void RemoveSubitems(wxTreeCtrl *browser);
    virtual bool DropObject(wxFrame *frame, wxTreeCtrl *browser) {return false; }
    virtual bool EditObject(wxFrame *frame, wxTreeCtrl *browser) {return false; }
    virtual int GetIcon()=0;

    virtual bool NeedCascadedDrop() { return false; }
    virtual bool CanCreate() { return false; }
    virtual bool CanView() { return false; }
    virtual bool CanEdit() { return false; }
    virtual bool CanDrop() { return false; }
    virtual bool CanVacuum() { return false; }
    virtual bool RequireDropConfirm() { return false; }

protected:
    void CreateListColumns(wxListCtrl *properties, const wxString &left=_("Property"), const wxString &right=_("Value"));
    void InsertListItem(wxListCtrl *list, const int pos, const wxString& str1, const wxString& str2);
    void InsertListItem(wxListCtrl *list, const int pos, const wxString& str1, const wxChar *s)
        { InsertListItem(list, pos, str1, wxString(s)); }
    void InsertListItem(wxListCtrl *list, const int pos, const wxString& str1, const long l)
        { InsertListItem(list, pos, str1, NumToStr(l)); }
    void InsertListItem(wxListCtrl *list, const int pos, const wxString& str1, const bool b)
        { InsertListItem(list, pos, str1, BoolToYesNo(b)); }
    void InsertListItem(wxListCtrl *list, const int pos, const wxString& str1, const double d)
        { InsertListItem(list, pos, str1, NumToStr(d)); }
    void InsertListItem(wxListCtrl *list, const int pos, const wxString& str1, const OID o)
        { InsertListItem(list, pos, str1, NumToStr(o)); }

    void AppendMenu(wxMenu *menu, int type=-1);
    virtual void SetContextInfo(frmMain *form) {}

    bool expandedKids, needReread;
    wxString sql;
    
private:
    static wxString GetPrivilegeGrant(const wxString& allPattern, const wxString& acl, const wxString& grantObject, const wxString& user);

    wxString name, owner, comment, acl;
    int type;
    OID oid;
};


// Object that lives under a server
class pgServerObject : public pgObject
{
public:
    pgServerObject(int newType, const wxString& newName) : pgObject(newType, newName) {}

    void iSetServer(pgServer *s) { server=s; }
    pgServer *GetServer() { return server; }

    bool CanCreate();
    bool CanDrop();
    bool CanEdit() { return true; }

protected:
    pgServer *server;
};



// Object that lives in a database
class pgDatabaseObject : public pgObject
{
public:
    pgDatabaseObject(int newType, const wxString& newName) : pgObject(newType, newName) {}

    pgDatabase *GetDatabase() const {return database; }
    void iSetDatabase(pgDatabase *newDatabase) { database = newDatabase; }

    bool CanDrop();
    bool CanEdit() { return true; }
    bool CanCreate();

protected:
    pgDatabase *database;
};



// Object that lives in a schema
class pgSchemaObject : public pgObject
{
public:
    pgSchemaObject(pgSchema *newSchema, int newType, const wxString& newName = wxT("")) : pgObject(newType, newName)
        { tableOid=0; schema = newSchema; wxLogInfo(wxT("Creating a pg") + GetTypeName() + wxT(" object")); }

    pgSchemaObject::~pgSchemaObject()
        { wxLogInfo(wxT("Destroying a pg") + GetTypeName() + wxT(" object")); }

    bool GetSystemObject() const;

    bool CanDrop();
    bool CanEdit() { return true; }
    bool CanCreate();

    void SetSchema(pgSchema *newSchema) { schema = newSchema; }
    pgSchema *GetSchema() const {return schema; }
    pgSet *ExecuteSet(const wxString& sql);
    wxString ExecuteScalar(const wxString& sql);
    bool ExecuteVoid(const wxString& sql);
    void DisplayStatistics(wxListCtrl *statistics, const wxString& query);
    OID GetTableOid() const {return tableOid; }
    void iSetTableOid(const OID d) { tableOid=d; }
    wxString GetTableOidStr() const {return NumToStr(tableOid) + wxT("::oid"); }
    virtual wxString GetFullIdentifier() const;
    virtual wxString GetQuotedFullIdentifier() const;


protected:
    virtual void SetContextInfo(frmMain *form);

    pgSchema *schema;
    OID tableOid;
};


class pgRuleObject : public pgSchemaObject
{
public:
    pgRuleObject(pgSchema *newSchema, int newType, const wxString& newName = wxT("")) 
        : pgSchemaObject(newSchema, newType, newName) {}

    wxString GetFormattedDefinition();
    wxString GetDefinition() const { return definition; }
    void iSetDefinition(const wxString& s) { definition=s; }

protected:
    wxString definition;
};
#endif

