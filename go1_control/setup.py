from setuptools import find_packages, setup

package_name = 'go1_control'

setup(
    name=package_name,
    version='0.0.1',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='chase',
    maintainer_email='chase@email.com',
    description='Go1 motor control',
    license='MIT',
    entry_points={
        'console_scripts': [
            'motor_control = go1_control.motor_control:main',
        ],
    },
)
