module OrganizationKeyCheckQuery = [%graphql
  {|
  query CheckOrganizationKey($key: String!) {
    isOrganizationKeyUsed(key: $key)
  }
|}
];

module FetchOrganizationKeyCheck =
  ReasonApollo.CreateQuery(OrganizationKeyCheckQuery);

module KeyCheckHandler = {
  let component = ReasonReact.statelessComponent("KeyCheckHandler");
  let make = (~isOrganizationKeyUsed, ~onChange, _children) => {
    ...component,
    /* Since the Apollo query is triggered using a declarative component we need
       to use a child component to trigger `onChange` updates each time the query
       returns a result */
    didMount: _self => {
      onChange(! isOrganizationKeyUsed);
      ();
    },
    didUpdate: _oldAndNewSelf => {
      onChange(! isOrganizationKeyUsed);
      ();
    },
    render: _self => {
      let hintText = if (isOrganizationKeyUsed) {"NO"} else {"OK"};
      hintText |> ReasonReact.string;
    },
  };
};

let component =
  ReasonReact.statelessComponentWithRetainedProps("OrganizationKeyCheck");

let make = (~value, ~onChange, _children) => {
  ...component,
  retainedProps: value,
  /* Only re-render if the value changed, to prevent infinite re-renders */
  shouldUpdate: ({oldSelf}) => oldSelf.retainedProps !== value,
  render: _self => {
    let organizationKeyCheckQuery =
      OrganizationKeyCheckQuery.make(~key=value, ());
    <FetchOrganizationKeyCheck variables=organizationKeyCheckQuery##variables>
      ...(
           ({result}) =>
             switch (result) {
             | Loading => "..." |> ReasonReact.string
             | Error(error) =>
               Js.log2("[KeyCheck] Error while fetching", error);
               ReasonReact.null;
             | Data(response) =>
               let isOrganizationKeyUsed = response##isOrganizationKeyUsed;
               <KeyCheckHandler isOrganizationKeyUsed onChange />;
             }
         )
    </FetchOrganizationKeyCheck>;
  },
};

let default =
  ReasonReact.wrapReasonForJs(~component, jsProps =>
    make(~value=jsProps##value, ~onChange=jsProps##onChange, [||])
  );