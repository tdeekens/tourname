module OrganizationQuery = {
  [@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";
  let query =
    [@bs]
    gql(
      {|
      query OrganizationQuery($key: String!) {
        organization(key: $key) {
          key
          name
          members {
            id
            createdAt
            lastModifiedAt
            email
            name
            picture
            isAdmin
          }
        }
      }
    |}
    );
  type member = {
    .
    "id": string,
    "createdAt": string,
    "email": string,
    "name": string,
    "picture": string,
    "isAdmin": bool
  };
  type organization = {
    .
    "key": string,
    "name": string,
    "members": list(member)
  };
  type data = {. "organization": organization};
  type response = data;
  type variables = {. "key": string };
};

module FetchOrganization = Apollo.Client.Query(OrganizationQuery);

let component = FetchOrganization.component;

let make = FetchOrganization.make;