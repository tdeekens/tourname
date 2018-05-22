import { withRouter } from 'react-router';
import gql from 'graphql-tag';
import { graphql } from 'react-apollo';
import { compose } from 'recompose';

const OrganizationQuery = gql`
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
`;

const withOrganization = mapDataToProps => Component =>
  compose(
    withRouter,
    graphql(OrganizationQuery, {
      alias: 'withOrganization',
      name: 'organization',
      props: ({ organization }) =>
        mapDataToProps ? mapDataToProps(organization) : { organization },
      options: ownProps => ({
        variables: {
          key:
            ownProps.match.params.organizationKey ||
            ownProps.fallbackOrganizationKey,
        },
      }),
    })
  )(Component);
export default withOrganization;