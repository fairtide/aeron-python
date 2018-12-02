#!/usr/bin/env bash

if [[ ! ${CIRCLE_TOKEN+x} ]]; then
    CURRENT_DIR=$(dirname "$0")
    CIRCLE_TOKEN=$(cat $CURRENT_DIR/.token)

    echo "TOKEN: $CIRCLE_TOKEN"
fi

REVISION=$(git rev-parse HEAD)
BRANCH_NAME=$(git branch | grep \* | cut -d ' ' -f2)
curl --user ${CIRCLE_TOKEN}: \
    --request POST \
    --form revision=${REVISION}\
    --form config=@config.yml \
    --form notify=false \
        https://circleci.com/api/v1.1/project/github/lukaszlaszko/pysyscall_intercept/tree/${BRANCH_NAME}
