/* Common types */
type any;

type actionFuncParams;

type obj = Js.Dict.t(string);


type noop = unit => unit;

/* Helpers */
let optionBoolToOptionJsBoolean = opt =>
  switch opt {
  | Some(true) => Some(Js.true_)
  | Some(false) => Some(Js.false_)
  | None => None
  };

[@bs.obj]
external getActionFunctionOptions :
  (~index: int=?, ~item: any=?, ~otherStateToSet: obj=?, ~cb: noop=?, unit) =>
  actionFuncParams =
  "";

/* Types for Downshift API */
type itemToString = any => string;

type selectedItemChanged = (any, any) => bool;

type a11yStatusMessageOptions = {
  .
  "highlightedIndex": option(int),
  "highlightedValue": any,
  "inputValue": string,
  "isOpen": bool,
  "itemToString": itemToString,
  "previousResultCount": int,
  "resultCount": int,
  "selectedItem": any
};

type getA11yStatusMessage = a11yStatusMessageOptions => string;

type onChange = (~selectedItem: any, ~stateAndHelpers: obj) => unit;

type onSelect = (~selectedItem: any, ~stateAndHelpers: obj) => unit;

type onStateChange = (~changes: obj, ~stateAndHelpers: obj) => unit;

type onInputValueChange = (~inputValue: string, ~stateAndHelpers: obj) => unit;

type rootPropsOptions = {. "refKey": string};

type itemPropsOptions = {
  .
  "index": option(int),
  "item": any
};

type renderFunc =
  {
    .
    /* Getters */
    "getRootProps": [@bs.meth] (rootPropsOptions => obj),
    "getButtonProps": [@bs.meth] (ReactDOMRe.reactDOMProps => obj),
    "getLabelProps": [@bs.meth] (ReactDOMRe.reactDOMProps => obj),
    "getInputProps": [@bs.meth] (ReactDOMRe.reactDOMProps => obj),
    "getItemProps": [@bs.meth] (itemPropsOptions => obj),
    /* Actions */
    "openMenu": [@bs.meth] (actionFuncParams => unit),
    "closeMenu": [@bs.meth] (actionFuncParams => unit),
    "toggleMenu": [@bs.meth] (actionFuncParams => unit),
    "reset": [@bs.meth] (actionFuncParams => unit),
    "selectItem": [@bs.meth] (actionFuncParams => unit),
    "selectItemAtIndex": [@bs.meth] (actionFuncParams => unit),
    "selectHighlightedItem": [@bs.meth] (actionFuncParams => unit),
    "setHighlightedIndex": [@bs.meth] (actionFuncParams => unit),
    "clearSelection": [@bs.meth] (actionFuncParams => unit),
    "clearItems": [@bs.meth] (unit => unit),
    "itemToString": [@bs.meth] (any => unit),
    /* State */
    "highlightedIndex": Js.nullable(int),
    "inputValue": Js.nullable(string),
    "isOpen": bool,
    "selectedItem": any
  } =>
  ReasonReact.reactElement;

module Downshift = {
  [@bs.module "downshift"]
  external reactClass : ReasonReact.reactClass = "default";
  let make =
      (
        ~defaultSelectedItem: option(any)=?,
        ~defaultHighlightedIndex: option(int)=?,
        ~defaultInputValue: option(string)=?,
        ~defaultIsOpen: option(bool)=?,
        ~itemToString: option(itemToString)=?,
        ~selectedItemChanged: option(selectedItemChanged)=?,
        ~getA11yStatusMessage: option(getA11yStatusMessage)=?,
        ~onChange: option(onChange)=?,
        ~onSelect: option(onSelect)=?,
        ~onStateChange: option(onStateChange)=?,
        ~onInputValueChange: option(onInputValueChange)=?,
        ~itemCount: option(int)=?,
        ~highlightedIndex: option(int)=?,
        ~inputValue: option(string)=?,
        ~isOpen: option(bool)=?,
        ~selectedItem: option(any)=?,
        ~render: renderFunc,
        ~id: option(string)=?,
        ~environment: option(Dom.window)=?,
        ~onOuterClick: option(noop)=?,
        _children
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props={
        "defaultSelectedItem": Js.Null_undefined.from_opt(defaultSelectedItem),
        "defaultHighlightedIndex":
          Js.Null_undefined.from_opt(defaultHighlightedIndex),
        "defaultInputValue": Js.Null_undefined.from_opt(defaultInputValue),
        "defaultIsOpen":
          Js.Null_undefined.from_opt(
            optionBoolToOptionJsBoolean(defaultIsOpen)
          ),
        "itemToString": Js.Null_undefined.from_opt(itemToString),
        "selectedItemChanged": Js.Null_undefined.from_opt(selectedItemChanged),
        "getA11yStatusMessage":
          Js.Null_undefined.from_opt(getA11yStatusMessage),
        "onChange": Js.Null_undefined.from_opt(onChange),
        "onSelect": Js.Null_undefined.from_opt(onSelect),
        "onStateChange": Js.Null_undefined.from_opt(onStateChange),
        "onInputValueChange": Js.Null_undefined.from_opt(onInputValueChange),
        "itemCount": Js.Null_undefined.from_opt(itemCount),
        "highlightedIndex": Js.Null_undefined.from_opt(highlightedIndex),
        "inputValue": Js.Null_undefined.from_opt(inputValue),
        "isOpen":
          Js.Null_undefined.from_opt(optionBoolToOptionJsBoolean(isOpen)),
        "selectedItem": Js.Null_undefined.from_opt(selectedItem),
        "render": render,
        "id": Js.Null_undefined.from_opt(id),
        "environment": Js.Null_undefined.from_opt(environment),
        "onOuterClick": Js.Null_undefined.from_opt(onOuterClick)
      },
      [||]
    );
};