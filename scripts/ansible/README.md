# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

This directory contains the Ansible work used to automate all the required tasks for setting up new Open Enclave environments, and new Jenkins agents for the CI / CD system.

To quickly install / uninstall Ansible, the script `install-ansible.sh` / `remove-ansible.sh` can be used.

# Open Enclave Deployment Options via Ansible

On the target machine where Open Enclave is desired to be configured, you may setup the environment in one of the following ways:

1. Open Enclave environment for contributors:

```
ansible-playbook oe-contributors-setup.yml
```

2. Open Enclave environment for contributors using ACC hardware:

```
ansible-playbook oe-contributors-acc-setup.yml
```

3. Open Enclave vanilla environment (without SGX packages and with Azure-DCAP-Client package)

```
ansible-playbook oe-vanilla-prelibsgx-setup.yml
```

# Configure new Jenkins slaves

The playbook `jenkins-setup.yml` can be used to set up new Jenkin slaves.

Before anything else, make sure you configure your [inventory](/scripts/ansible/inventory) accordingly to target the new machines.

As a good practice, it is advised to address the nodes via their hostnames directly (for simplicity purposes). If the hostnames are not resolvable by the configured DNS server, before running the playbook append a new line to /etc/hosts containing `"NEW_SLAVE_IP"    "NEW_SLAVE_NAME"`. For example:

```
10.0.28.143    ACC-1604-5
```

At the moment, the Jenkins setup playbook has a third party dependency on `kobanyan.jenkins-jnlp-slave`. Before running the main playbook, make sure this is installed:

```
ansible-galaxy install kobanyan.jenkins-jnlp-slave
```

The playbook can be started with:

```
ansible-playbook jenkins-setup.yml
```

# Supported platforms by the Ansible playbooks

* Ubuntu 16.04
* Ubuntu 18.04
